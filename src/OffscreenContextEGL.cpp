/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2025, Christoph Neuhauser
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstring>
#include <Utils/StringUtils.hpp>
#include <Utils/File/Logfile.hpp>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "OffscreenContextCommon.hpp"
#include "OffscreenContextEGL.hpp"

#define GLAPIENTRY EGLAPIENTRY
#include "GLCommon.hpp"

#include <Graphics/Vulkan/Utils/Device.hpp>

#if defined(__linux__)
#include <dlfcn.h>
#include <unistd.h>
#elif defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif defined(__APPLE__)
#include <dlfcn.h>
#endif

struct OffscreenContextEGLFunctionTable {
    PFNEGLGETPROCADDRESSPROC eglGetProcAddress;
    PFNEGLGETERRORPROC eglGetError;
    PFNEGLQUERYSTRINGPROC eglQueryString;
    PFNEGLGETDISPLAYPROC eglGetDisplay;
    PFNEGLINITIALIZEPROC eglInitialize;
    PFNEGLCHOOSECONFIGPROC eglChooseConfig;
    PFNEGLCREATEPBUFFERSURFACEPROC eglCreatePbufferSurface;
    PFNEGLBINDAPIPROC eglBindAPI;
    PFNEGLCREATECONTEXTPROC eglCreateContext;
    PFNEGLDESTROYSURFACEPROC eglDestroySurface;
    PFNEGLDESTROYCONTEXTPROC eglDestroyContext;
    PFNEGLTERMINATEPROC eglTerminate;
    PFNEGLMAKECURRENTPROC eglMakeCurrent;

    // EXT functions are optional.
    PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT;
    PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT;
    PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
    PFNEGLQUERYDEVICEBINARYEXTPROC eglQueryDeviceBinaryEXT;
};

#ifdef _WIN32
static HMODULE eglHandle = nullptr;
#else
static void* eglHandle = nullptr;
#endif
static OffscreenContextEGLFunctionTable* eglf = nullptr;
static EGLint numEglDevices = 0;
static EGLDeviceEXT* eglDevices = nullptr;

bool loadEglLibrary() {
#if defined(__linux__)
    eglHandle = dlopen("libEGL.so", RTLD_NOW | RTLD_LOCAL);
    if (!eglHandle) {
        eglHandle = dlopen("libEGL.so.1", RTLD_NOW | RTLD_LOCAL);
        if (!eglHandle) {
            sgl::Logfile::get()->write("loadEglLibrary: Could not load libEGL.so.", sgl::ORANGE);
            return false;
        }
    }
#elif defined(_WIN32)
    eglHandle = LoadLibraryA("EGL.dll");
    if (!eglHandle) {
        sgl::Logfile::get()->write("loadEglLibrary: Could not load EGL.dll.", sgl::ORANGE);
        return false;
    }
#endif

    eglf = new OffscreenContextEGLFunctionTable;
    eglf->eglGetProcAddress = PFNEGLGETPROCADDRESSPROC(dlsym(eglHandle, TOSTRING(eglGetProcAddress)));
    eglf->eglGetError = PFNEGLGETERRORPROC(dlsym(eglHandle, TOSTRING(eglGetError)));
    eglf->eglQueryString = PFNEGLQUERYSTRINGPROC(dlsym(eglHandle, TOSTRING(eglQueryString)));
    eglf->eglGetDisplay = PFNEGLGETDISPLAYPROC(dlsym(eglHandle, TOSTRING(eglGetDisplay)));
    eglf->eglInitialize = PFNEGLINITIALIZEPROC(dlsym(eglHandle, TOSTRING(eglInitialize)));
    eglf->eglChooseConfig = PFNEGLCHOOSECONFIGPROC(dlsym(eglHandle, TOSTRING(eglChooseConfig)));
    eglf->eglCreatePbufferSurface = PFNEGLCREATEPBUFFERSURFACEPROC(dlsym(eglHandle, TOSTRING(eglCreatePbufferSurface)));
    eglf->eglBindAPI = PFNEGLBINDAPIPROC(dlsym(eglHandle, TOSTRING(eglBindAPI)));
    eglf->eglCreateContext = PFNEGLCREATECONTEXTPROC(dlsym(eglHandle, TOSTRING(eglCreateContext)));
    eglf->eglDestroySurface = PFNEGLDESTROYSURFACEPROC(dlsym(eglHandle, TOSTRING(eglDestroySurface)));
    eglf->eglDestroyContext = PFNEGLDESTROYCONTEXTPROC(dlsym(eglHandle, TOSTRING(eglDestroyContext)));
    eglf->eglTerminate = PFNEGLTERMINATEPROC(dlsym(eglHandle, TOSTRING(eglTerminate)));
    eglf->eglMakeCurrent = PFNEGLMAKECURRENTPROC(dlsym(eglHandle, TOSTRING(eglMakeCurrent)));

    if (!eglf->eglGetProcAddress
            || !eglf->eglGetError
            || !eglf->eglQueryString
            || !eglf->eglGetDisplay
            || !eglf->eglInitialize
            || !eglf->eglChooseConfig
            || !eglf->eglCreatePbufferSurface
            || !eglf->eglBindAPI
            || !eglf->eglCreateContext
            || !eglf->eglDestroySurface
            || !eglf->eglDestroyContext
            || !eglf->eglTerminate
            || !eglf->eglMakeCurrent) {
        sgl::Logfile::get()->writeError(
                "Error in loadEglLibrary: At least one function pointer could not be loaded.", false);
        delete eglf;
        return false;
    }

    // EXT functions are optional.
    eglf->eglQueryDevicesEXT = PFNEGLQUERYDEVICESEXTPROC(eglf->eglGetProcAddress(TOSTRING(eglQueryDevicesEXT)));
    eglf->eglQueryDeviceStringEXT = PFNEGLQUERYDEVICESTRINGEXTPROC(eglf->eglGetProcAddress(TOSTRING(eglQueryDeviceStringEXT)));
    eglf->eglGetPlatformDisplayEXT = PFNEGLGETPLATFORMDISPLAYEXTPROC(eglf->eglGetProcAddress(TOSTRING(eglGetPlatformDisplayEXT)));
    eglf->eglQueryDeviceBinaryEXT = PFNEGLQUERYDEVICEBINARYEXTPROC(eglf->eglGetProcAddress(TOSTRING(eglQueryDeviceBinaryEXT)));

    const char* extensionsNoDisplay = eglf->eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    std::string extensionsNoDisplayString(extensionsNoDisplay);
    sgl::Logfile::get()->write(
            "EGL extensions for EGL_NO_DISPLAY: " + extensionsNoDisplayString, sgl::BLUE);

    numEglDevices = 0;
    if (!eglf->eglQueryDevicesEXT(0, nullptr, &numEglDevices)) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglQueryDevicesEXT failed.", false);
        return false;
    }
    eglDevices = new EGLDeviceEXT[numEglDevices];
    if (!eglf->eglQueryDevicesEXT(numEglDevices, eglDevices, &numEglDevices)) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglQueryDevicesEXT failed.", false);
        return false;
    }

    return true;
}

void releaseEglLibrary() {
    if (eglf) {
        delete eglf;
        eglf = nullptr;
    }
    if (eglHandle) {
#if defined(__linux__)
        dlclose(eglHandle);
#elif defined(_WIN32)
        FreeLibrary(eglHandle);
#endif
        eglHandle = {};
    }
}

void* getEglFunctionPointer(const char* functionName) {
    if (!eglf) {
        return nullptr;
    }
    return (void*)eglf->eglGetProcAddress(functionName);
}
void checkEglFeatures(sgl::vk::Device* device) {
    if (!eglf->eglQueryDevicesEXT || !eglf->eglQueryDeviceStringEXT
            || !eglf->eglGetPlatformDisplayEXT || !eglf->eglQueryDeviceBinaryEXT) {
        return;
    }
    const VkPhysicalDeviceIDProperties& physicalDeviceIdProperties = device->getDeviceIDProperties();

    int matchingDeviceIdx = -1;
    for (int i = 0; i < numEglDevices; i++) {
        const char* deviceExtensions = eglf->eglQueryDeviceStringEXT(eglDevices[i], EGL_EXTENSIONS);
        if (!deviceExtensions) {
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextEGL::initialize: eglQueryDeviceStringEXT failed.", false);
            return;
        }
        std::string deviceExtensionsString(deviceExtensions);
        std::vector<std::string> deviceExtensionsVector;
        sgl::splitStringWhitespace(deviceExtensionsString, deviceExtensionsVector);
        std::set<std::string> deviceExtensionsSet(deviceExtensionsVector.begin(), deviceExtensionsVector.end());

        if (deviceExtensionsSet.find("EGL_EXT_device_persistent_id") == deviceExtensionsSet.end()) {
            //sgl::Logfile::get()->write(
            //        "Discarding EGL device #" + std::to_string(i)
            //        + " due to not supporting EGL_EXT_device_persistent_id.", sgl::BLUE);
            continue;
        }

        const size_t UUID_SIZE = VK_UUID_SIZE;
        uint8_t deviceUuid[VK_UUID_SIZE];
        EGLint uuidSize = 0;
        if (eglf->eglQueryDeviceBinaryEXT && !eglf->eglQueryDeviceBinaryEXT(
                eglDevices[i], EGL_DEVICE_UUID_EXT, EGLint(VK_UUID_SIZE), deviceUuid, &uuidSize)) {
            EGLint errorCode = eglf->eglGetError();
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextEGL::initialize: eglQueryDeviceBinaryEXT failed (error code: "
                    + std::to_string(errorCode) + ").", false);
            return;
        }
        if (eglf->eglQueryDeviceBinaryEXT && strncmp(
                (const char*)deviceUuid, (const char*)physicalDeviceIdProperties.deviceUUID, UUID_SIZE) == 0) {
            matchingDeviceIdx = i;
            //break;
        }
    }

    if (matchingDeviceIdx < 0) {
        return;
    }

    sgl::Logfile::get()->write("<br>\n");
    const char* deviceExtensions = eglf->eglQueryDeviceStringEXT(eglDevices[matchingDeviceIdx], EGL_EXTENSIONS);
    if (!deviceExtensions) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglQueryDeviceStringEXT failed.", false);
        return;
    }
    std::string deviceExtensionsString(deviceExtensions);
    sgl::Logfile::get()->write("Device EGL extensions: " + deviceExtensionsString, sgl::BLUE);
    std::vector<std::string> deviceExtensionsVector;
    sgl::splitStringWhitespace(deviceExtensionsString, deviceExtensionsVector);
    std::set<std::string> deviceExtensionsSet(deviceExtensionsVector.begin(), deviceExtensionsVector.end());
    if (deviceExtensionsSet.find("EGL_EXT_device_query_name") != deviceExtensionsSet.end()) {
        const char* deviceVendor = eglf->eglQueryDeviceStringEXT(eglDevices[matchingDeviceIdx], EGL_VENDOR);
        const char* deviceRenderer = eglf->eglQueryDeviceStringEXT(eglDevices[matchingDeviceIdx], EGL_RENDERER_EXT);
        if (deviceVendor) {
            sgl::Logfile::get()->write(std::string() + "Device EGL vendor: " + deviceVendor, sgl::BLUE);
        }
        if (deviceRenderer) {
            sgl::Logfile::get()->write(std::string() + "Device EGL renderer: " + deviceRenderer, sgl::BLUE);
        }
    }

    if (deviceExtensionsSet.find("EGL_EXT_device_persistent_id") != deviceExtensionsSet.end()) {
        const char* deviceDriverName = eglf->eglQueryDeviceStringEXT(eglDevices[matchingDeviceIdx], EGL_DRIVER_NAME_EXT);
        if (deviceDriverName) {
            sgl::Logfile::get()->write(std::string() + "Device EGL driver: " + deviceDriverName, sgl::BLUE);
        }
    }

    if (deviceExtensionsSet.find("EGL_EXT_device_drm") != deviceExtensionsSet.end()) {
        const char* deviceDrmFile = eglf->eglQueryDeviceStringEXT(eglDevices[matchingDeviceIdx], EGL_DRM_DEVICE_FILE_EXT);
        if (deviceDrmFile) {
            sgl::Logfile::get()->write(std::string() + "Device EGL DRM file: " + deviceDrmFile, sgl::BLUE);
        }
    }

    if (deviceExtensionsSet.find("EGL_EXT_device_drm_render_node") != deviceExtensionsSet.end()) {
        const char* deviceDrmRenderNodeFile = eglf->eglQueryDeviceStringEXT(
                eglDevices[matchingDeviceIdx], EGL_DRM_RENDER_NODE_FILE_EXT);
        if (deviceDrmRenderNodeFile) {
            sgl::Logfile::get()->write(
                    std::string() + "Device EGL DRM render node file: " + deviceDrmRenderNodeFile, sgl::BLUE);
        }
    }

    EGLDisplay eglDisplay = eglf->eglGetPlatformDisplayEXT(
        EGL_PLATFORM_DEVICE_EXT, eglDevices[matchingDeviceIdx], nullptr);

    if (!eglDisplay) {
        EGLint errorCode = eglf->eglGetError();
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglGetPlatformDisplayEXT failed (error code: "
                + std::to_string(errorCode) + ").", false);
        return;
    }

    const char* extensionsDeviceDisplay = eglf->eglQueryString(eglDisplay, EGL_EXTENSIONS);
    if (extensionsDeviceDisplay) {
        std::string extensionsDeviceDisplayString(extensionsDeviceDisplay);
        sgl::Logfile::get()->write("Device EGL extensions: " + extensionsDeviceDisplayString, sgl::BLUE);
    }

    EGLint major, minor;
    if (!eglf->eglInitialize(eglDisplay, &major, &minor)) {
        sgl::Logfile::get()->writeError("Error in OffscreenContextEGL::initialize: eglInitialize failed.", false);
        return;
    }
    sgl::Logfile::get()->write(
        "EGL display version: " + std::to_string(major) + "." + std::to_string(minor), sgl::BLUE);

    const char* displayVendor = eglf->eglQueryString(eglDisplay, EGL_VENDOR);
    sgl::Logfile::get()->write(std::string() + "EGL display vendor: " + displayVendor, sgl::BLUE);

    EGLint numConfigs;
    EGLConfig eglConfig;
    bool resultEglChooseConfig;
    constexpr EGLint configAttributes[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    resultEglChooseConfig = eglf->eglChooseConfig(eglDisplay, configAttributes, &eglConfig, 1, &numConfigs);

    if (numConfigs <= 0) {
        sgl::Logfile::get()->writeError("Error in OffscreenContextEGL::initialize: eglChooseConfig returned 0.", false);
        return;
    }
    if (!resultEglChooseConfig) {
        sgl::Logfile::get()->writeError("Error in OffscreenContextEGL::initialize: eglChooseConfig failed.", false);
        return;
    }

    int pbufferWidth = 32;
    int pbufferHeight = 32;
    static const EGLint pbufferAttributes[] = {
        EGL_WIDTH, pbufferWidth,
        EGL_HEIGHT, pbufferHeight,
        EGL_NONE,
    };
    EGLSurface eglSurface = eglf->eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttributes);
    if (!eglSurface) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglCreatePbufferSurface failed.", false);
        if (eglDisplay) {
            if (!eglf->eglTerminate(eglDisplay)) {
                sgl::Logfile::get()->writeError(
                        "Error in OffscreenContextEGL::~OffscreenContextEGL: eglTerminate failed.", true);
            }
            eglDisplay = {};
        }
        return;
    }

    if (!eglf->eglBindAPI(EGL_OPENGL_API)) {
        sgl::Logfile::get()->writeError("Error in OffscreenContextEGL::initialize: eglBindAPI failed.", false);
        return;
    }

    EGLContext eglContext = eglf->eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, nullptr);
    if (!eglContext) {
        EGLint errorCode = eglf->eglGetError();
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::initialize: eglCreateContext failed (error code: "
                + std::to_string(errorCode) + ").", false);
        if (eglSurface) {
            if (!eglf->eglDestroySurface(eglDisplay, eglSurface)) {
                sgl::Logfile::get()->writeError(
                        "Error in OffscreenContextEGL::~OffscreenContextEGL: eglDestroySurface failed.", true);
            }
            eglSurface = {};
        }
        if (eglDisplay) {
            if (!eglf->eglTerminate(eglDisplay)) {
                sgl::Logfile::get()->writeError(
                        "Error in OffscreenContextEGL::~OffscreenContextEGL: eglTerminate failed.", true);
            }
            eglDisplay = {};
        }
        return;
    }

    EGLBoolean retVal;
    if (!eglSurface) {
        retVal = eglf->eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, eglContext);
    } else {
        retVal = eglf->eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    }
    if (!retVal) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextEGL::makeCurrent: eglMakeCurrent failed.", true);
    }

    printOpenGLContextInformation(getEglFunctionPointer);

    if (eglSurface) {
        if (!eglf->eglDestroySurface(eglDisplay, eglSurface)) {
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextEGL::~OffscreenContextEGL: eglDestroySurface failed.", true);
        }
        eglSurface = {};
    }
    if (eglContext) {
        if (!eglf->eglDestroyContext(eglDisplay, eglContext)) {
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextEGL::~OffscreenContextEGL: eglDestroyContext failed.", true);
        }
        eglContext = {};
    }
    if (eglDisplay) {
        if (!eglf->eglTerminate(eglDisplay)) {
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextEGL::~OffscreenContextEGL: eglTerminate failed.", true);
        }
        eglDisplay = {};
    }
}
