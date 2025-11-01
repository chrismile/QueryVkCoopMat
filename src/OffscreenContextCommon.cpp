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

#include <string>
#include <Utils/File/Logfile.hpp>
#include <ImGui/Widgets/NumberFormatting.hpp>

#include <EGL/egl.h>

#define GLAPIENTRY EGLAPIENTRY
#include "GLCommon.hpp"
#include "OffscreenContextCommon.hpp"

bool printOpenGLContextInformation(void* (*getGlFunctionPointer)(const char* functionName)) {
    auto* glGetString = PFNGLGETSTRINGPROC(getGlFunctionPointer("glGetString"));
    auto* glGetStringi = PFNGLGETSTRINGIPROC(getGlFunctionPointer("glGetStringi"));
    auto* glGetIntegerv = PFNGLGETINTEGERVPROC(getGlFunctionPointer("glGetIntegerv"));
    auto* glGetInteger64v = PFNGLGETINTEGER64VPROC(getGlFunctionPointer("glGetInteger64v"));
    if (!glGetString || !glGetStringi || !glGetIntegerv) {
        sgl::Logfile::get()->writeError(
                "Error in OffscreenContextWGL::makeCurrent: getGlFunctionPointer failed.", true);
        return false;
    }

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
            std::string() + "OpenGL Shading Language Version: "
            + (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION), sgl::BLUE);
    sgl::Logfile::get()->write(
            std::string() + "OpenGL Extensions: " + extensionString, sgl::BLUE);

    int64_t maxShaderStorageBlockSize;
    GLint ssboOffsetAlignment;
    glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxShaderStorageBlockSize);
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &ssboOffsetAlignment);
    sgl::Logfile::get()->write(
            std::string() + "OpenGL SSBO Max Size: "
            + sgl::getNiceMemoryStringDifference(uint64_t(maxShaderStorageBlockSize), 2, true), sgl::BLUE);
    sgl::Logfile::get()->write(
            std::string() + "OpenGL SSBO Offset Alignment: "
            + sgl::getNiceMemoryString(uint64_t(ssboOffsetAlignment), 2), sgl::BLUE);

    return true;
}
