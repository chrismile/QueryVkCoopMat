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

// BEGIN D3D12
#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d12.h>
#include <dxgi1_6.h>

inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        throw std::runtime_error("Error: D3D12 called failed.");
    }
}
// END D3D12

#include <GL/gl.h>
#include <GL/wgl.h>
#include <GL/wglext.h>

#define GLAPIENTRY APIENTRY
#include "GLCommon.hpp"

#include "OffscreenContextWGL.hpp"

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

struct OffscreenContextWGLFunctionTable {
    PFNWGLCREATECONTEXTPROC wglCreateContext;
    PFNWGLDELETECONTEXTPROC wglDeleteContext;
    PFNWGLMAKECURRENTPROC wglMakeCurrent;
    PFNWGLGETPROCADDRESSPROC wglGetProcAddress;
    PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
};

static HMODULE user32Module = nullptr;
static HMODULE openglModule = nullptr;
typedef BOOL ( WINAPI *PFN_EnumDisplayDevicesA )( LPCSTR lpDevice, DWORD iDevNum, PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags );
static OffscreenContextWGLFunctionTable* wglf = nullptr;

bool loadWglFunctions() {
    openglModule = LoadLibrary("opengl32.dll");
    if (!openglModule) {
        sgl::Logfile::get()->write("loadWglFunctions: Could not load opengl32.dll.", sgl::ORANGE);
        return false;
    }

    wglf = new OffscreenContextWGLFunctionTable;
    wglf->wglCreateContext = PFNWGLCREATECONTEXTPROC(dlsym(openglModule, TOSTRING(wglCreateContext)));
    wglf->wglDeleteContext = PFNWGLDELETECONTEXTPROC(dlsym(openglModule, TOSTRING(wglDeleteContext)));
    wglf->wglMakeCurrent = PFNWGLMAKECURRENTPROC(dlsym(openglModule, TOSTRING(wglMakeCurrent)));
    wglf->wglGetProcAddress = PFNWGLGETPROCADDRESSPROC(dlsym(openglModule, TOSTRING(wglGetProcAddress)));
    if (!wglf->wglCreateContext
            || !wglf->wglDeleteContext
            || !wglf->wglMakeCurrent
            || !wglf->wglGetProcAddress) {
        sgl::Logfile::get()->writeError(
                "Error in loadWglFunctions: At least one function pointer could not be loaded.", false);
        delete wglf;
        return false;
    }
    return true;
}

void* getWglFunctionPointer(const char* functionName) {
    void* p = (void*)wglf->wglGetProcAddress(functionName);
    if (p == nullptr || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1)) {
        p = (void*)GetProcAddress(openglModule, functionName);
    }
    return p;
}

void cleanupWgl(HGLRC glRenderingContext) {
    wglf->wglDeleteContext(glRenderingContext);
    delete wglf;
    wglf = nullptr;
    FreeLibrary(openglModule);
}

void checkDefaultWglContext() {
    const char* windowClassName = "wglwindowclass";
    const char* windowName = "wglwindowname";
    WNDCLASS wc{};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wc.lpszClassName = windowClassName;
    wc.style = CS_OWNDC;
    if (!RegisterClass(&wc)) {
        return;
    }
    HWND hWnd = CreateWindowExA(
            0, windowClassName, windowName, WS_OVERLAPPEDWINDOW,
            0, 0, 640, 480, nullptr, nullptr, nullptr, nullptr);
    HDC deviceContext = GetDC(hWnd);

    if (!loadWglFunctions()) {
        return;
    }

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER_DONTCARE | PFD_STEREO_DONTCARE
            | PFD_DEPTH_DONTCARE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    int pfi = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pfi, &pfd);

    HGLRC glRenderingContext = wglf->wglCreateContext(deviceContext);
    wglf->wglMakeCurrent(deviceContext, glRenderingContext);

    sgl::Logfile::get()->write("<br><hr><br>\n");
    sgl::Logfile::get()->write("Default WGL OpenGL context information", sgl::BLUE);
    auto* glGetString = PFNGLGETSTRINGPROC(getWglFunctionPointer("glGetString"));
    auto* glGetStringi = PFNGLGETSTRINGIPROC(getWglFunctionPointer("glGetStringi"));
    auto* glGetIntegerv = PFNGLGETINTEGERVPROC(getWglFunctionPointer("glGetIntegerv"));
    if (!glGetString || !glGetStringi || !glGetIntegerv) {
        sgl::Logfile::get()->writeError(
                "Error in checkDefaultWglContext: getWglFunctionPointer failed.", true);
    } else {
        sgl::Logfile::get()->write("<br>\n");
        sgl::Logfile::get()->write(
                std::string() + "OpenGL Version: " + (const char*)glGetString(GL_VERSION), sgl::BLUE);
        sgl::Logfile::get()->write(
                std::string() + "OpenGL Vendor: " + (const char*)glGetString(GL_VENDOR), sgl::BLUE);
        sgl::Logfile::get()->write(
                std::string() + "OpenGL Renderer: " + (const char*)glGetString(GL_RENDERER), sgl::BLUE);
        sgl::Logfile::get()->write(
                std::string() + "OpenGL Shading Language Version: " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION), sgl::BLUE);
    }

    cleanupWgl(glRenderingContext);
    DestroyWindow(hWnd);
}

// --- Code adapted from https://community.khronos.org/t/how-to-use-opengl-with-a-device-chosen-by-you/63017/6 ---
HWND WINAPI windowFromDeviceContextReplacement(HDC dc) {
    static HWND wnd = nullptr;

    if (dc == nullptr) {
        return nullptr;
    }

    if (wnd == nullptr) {
        WNDCLASSA wc;
        memset(&wc, 0, sizeof(wc));
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = GetModuleHandleA(nullptr);
        wc.lpszClassName = "dummy_window_class";
        RegisterClassA(&wc);
        wnd = CreateWindowA(wc.lpszClassName, nullptr, WS_POPUP, 0, 0, 32, 32, nullptr, nullptr, wc.hInstance, nullptr);
    }

    return wnd;
}

void patchWindowFromDeviceContext() {
    DWORD old_prot;
    auto wfdc = (unsigned __int64)GetProcAddress(user32Module, "WindowFromDC");

    VirtualProtect((void*)wfdc, 14, PAGE_EXECUTE_WRITECOPY, &old_prot);

    // jmp [eip + 0]
    *(char *)(wfdc + 0) = char(0xFF);
    *(char *)(wfdc + 1) = char(0x25);
    *(unsigned *)(wfdc + 2) = 0x00000000;
    *(unsigned __int64 *)(wfdc + 6) = (unsigned __int64)&windowFromDeviceContextReplacement;
}
// --- END ---

bool initWglPatch() {
    user32Module = GetModuleHandleA("user32.dll");
    checkDefaultWglContext();
    patchWindowFromDeviceContext();
    return true;
}

void freeWglPatch() {
    if (user32Module) {
        FreeLibrary(user32Module);
        user32Module = {};
    }
}

void checkWglFeatures(sgl::vk::Device* device) {
    const VkPhysicalDeviceIDProperties& physicalDeviceIdProperties = device->getDeviceIDProperties();
    if (!physicalDeviceIdProperties.deviceLUIDValid) {
        return;
    }

    /*
     * The name of the display adapter associated with the GPU (e.g., \\.\DISPLAY1).
     * On Windows, multiple display adapters may exist for the same GPU.
     * Each display adapter may have multiple display monitors attached (e.g., \\.\DISPLAY1\Monitor0).
     * The name of the display adapter can be used with CreateDCA and the above patching code to create a suitable
     * OpenGL context.
     */
    std::string displayName;

    /*
     * D3D12 allows querying the display adapter name if an output display is connected to the adapter.
     * Thus, we go the route Vulkan LUID -> D3D12 adapter -> display adapter name.
     */
    ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags = 0;
    ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));
    ComPtr<IDXGIAdapter1> dxgiAdapter1;
    ComPtr<IDXGIAdapter4> dxgiAdapter4;
    for (UINT adapterIdx = 0; dxgiFactory->EnumAdapters1(adapterIdx, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++adapterIdx) {
        DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
        dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);
        bool isSoftwareRenderer = (dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0;
        if (isSoftwareRenderer) {
            continue;
        }
        static_assert(VK_LUID_SIZE == sizeof(LUID));
        if (strncmp(
                (const char*)&dxgiAdapterDesc1.AdapterLuid, (const char*)physicalDeviceIdProperties.deviceLUID,
                VK_LUID_SIZE) != 0) {
            continue;
        }

        /*
         * VendorId (https://gamedev.stackexchange.com/questions/31625/get-video-chipset-manufacturer-in-direct3d):
         * NVIDIA: 0x10DE
         * AMD: 0x1002
         * Intel: 0x8086
         */
        //dxgiAdapterDesc1.VendorId;
        ComPtr<IDXGIOutput> out;
        for (size_t odx = 0; dxgiAdapter1->EnumOutputs(odx, &out) == S_OK; odx++) {
            DXGI_OUTPUT_DESC desc2;
            out->GetDesc(&desc2);
            displayName = sgl::wideStringArrayToStdString(desc2.DeviceName);
            break;
        }
    }

    if (displayName.empty()) {
        // This physical device (D3D12: adapter) is not associated with a display. Use a name matching heuristic.
        auto* pEnumDisplayDevicesA = PFN_EnumDisplayDevicesA(GetProcAddress(user32Module, "EnumDisplayDevicesA"));
        DWORD adapterIdx = 0;
        DISPLAY_DEVICEA displayDevice{};
        displayDevice.cb = sizeof(DISPLAY_DEVICEA);
        do {
            bool retVal = pEnumDisplayDevicesA(nullptr, adapterIdx, &displayDevice, 0);
            if (!retVal) {
                adapterIdx++;
                break;
            }
            std::cout << "--- Device #" << adapterIdx << " display adapter ---" << std::endl;
            if (strcmp(device->getPhysicalDeviceProperties().deviceName, displayDevice.DeviceString) == 0) {
                displayName = displayDevice.DeviceName;
                break;
            }
            adapterIdx++;
        } while (true);
    }

    if (displayName.empty()) {
        // Could not match Vulkan device to display adapter.
        return;
    }

    if (!loadWglFunctions()) {
        return;
    }

    HDC deviceContext = CreateDCA(displayName.c_str(), displayName.c_str(), nullptr, nullptr);

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER_DONTCARE | PFD_STEREO_DONTCARE
            | PFD_DEPTH_DONTCARE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    int pfi = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pfi, &pfd);

    HGLRC glRenderingContext = wglf->wglCreateContext(deviceContext);
    wglf->wglMakeCurrent(deviceContext, glRenderingContext);
    wglf->wglGetExtensionsStringARB = PFNWGLGETEXTENSIONSSTRINGARBPROC(
            getWglFunctionPointer(TOSTRING(wglGetExtensionsStringARB)));

    std::set<std::string> deviceExtensionsSet;
    std::string deviceExtensionsString;
    if (wglf->wglGetExtensionsStringARB) {
        const char* deviceExtensions = wglf->wglGetExtensionsStringARB(deviceContext);
        if (!deviceExtensions) {
            sgl::Logfile::get()->writeError(
                    "Error in OffscreenContextWGL::initialize: wglGetExtensionsStringARB failed.", false);
            cleanupWgl(glRenderingContext);
            return;
        }
        deviceExtensionsString = deviceExtensions;
        std::vector<std::string> deviceExtensionsVector;
        sgl::splitStringWhitespace(deviceExtensionsString, deviceExtensionsVector);
        deviceExtensionsSet = std::set<std::string>(deviceExtensionsVector.begin(), deviceExtensionsVector.end());
    }

    if (deviceExtensionsSet.find("WGL_NV_gpu_affinity") != deviceExtensionsSet.end()) {
        // TODO
    }
    if (deviceExtensionsSet.find("WGL_AMD_gpu_association") != deviceExtensionsSet.end()) {
        // TODO
    }

    auto* glGetString = PFNGLGETSTRINGPROC(getWglFunctionPointer("glGetString"));
    auto* glGetStringi = PFNGLGETSTRINGIPROC(getWglFunctionPointer("glGetStringi"));
    auto* glGetIntegerv = PFNGLGETINTEGERVPROC(getWglFunctionPointer("glGetIntegerv"));
    auto* glGetUnsignedBytevEXT = PFNGLGETUNSIGNEDBYTEVEXTPROC(getWglFunctionPointer("glGetUnsignedBytevEXT"));
    auto* glGetUnsignedBytei_vEXT = PFNGLGETUNSIGNEDBYTEI_VEXTPROC(getWglFunctionPointer("glGetUnsignedBytei_vEXT"));
    if (!glGetUnsignedBytevEXT || !glGetUnsignedBytei_vEXT) {
        cleanupWgl(glRenderingContext);
        return;
    }

    GLubyte deviceUuid[GL_UUID_SIZE_EXT];
    GLubyte driverUuid[GL_UUID_SIZE_EXT];
    glGetUnsignedBytevEXT(GL_DRIVER_UUID_EXT, driverUuid);
    if (strncmp((const char*)driverUuid, (const char*)physicalDeviceIdProperties.driverUUID, VK_UUID_SIZE) != 0) {
        //sgl::Logfile::get()->write(
        //        "Discarding the Vulkan device due to a mismatch in driver UUIDs with the OpenGL context.",
        //        BLUE);
        cleanupWgl(glRenderingContext);
        return;
    }

    GLint numDevices = 0;
    glGetIntegerv(GL_NUM_DEVICE_UUIDS_EXT, &numDevices);
    for (int deviceIdx = 0; deviceIdx < numDevices; deviceIdx++) {
        glGetUnsignedBytei_vEXT(GL_DEVICE_UUID_EXT, deviceIdx, deviceUuid);
        if (strncmp((const char*)deviceUuid, (const char*)physicalDeviceIdProperties.deviceUUID, VK_UUID_SIZE) == 0) {
            break;
        }
    }

    if (wglf->wglGetExtensionsStringARB) {
        sgl::Logfile::get()->write("<br>\n");
        sgl::Logfile::get()->write("Device WGL extensions: " + deviceExtensionsString, sgl::BLUE);
    }

    if (!glGetString || !glGetStringi || !glGetIntegerv) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextWGL::makeCurrent: getWglFunctionPointer failed.", true);
    } else {
        int n = 0;
        std::string extensionString;
        glGetIntegerv(GL_NUM_EXTENSIONS, &n);
        for (int i = 0; i < n; i++) {
            std::string extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
            extensionString += extension;
            if (i + 1 < n) {
                extensionString += ", ";
            }
        }
        sgl::Logfile::get()->write("<br>\n");
        sgl::Logfile::get()->write(
            std::string() + "OpenGL Version: " + (const char*)glGetString(GL_VERSION), sgl::BLUE);
        sgl::Logfile::get()->write(
            std::string() + "OpenGL Vendor: " + (const char*)glGetString(GL_VENDOR), sgl::BLUE);
        sgl::Logfile::get()->write(
            std::string() + "OpenGL Renderer: " + (const char*)glGetString(GL_RENDERER), sgl::BLUE);
        sgl::Logfile::get()->write(
            std::string() + "OpenGL Shading Language Version: " + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION), sgl::BLUE);
        sgl::Logfile::get()->write(
            std::string() + "OpenGL Extensions: " + extensionString, sgl::BLUE);
    }

    //wglf->wglMakeCurrent(deviceContext, nullptr); // already done by wglDeleteContext
    cleanupWgl(glRenderingContext);
}
