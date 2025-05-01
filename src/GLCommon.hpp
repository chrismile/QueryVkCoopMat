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

#ifndef QUERYVKCOOPMAT_GLCOMMON_HPP
#define QUERYVKCOOPMAT_GLCOMMON_HPP

#include <cstdint>
#include <GL/gl.h>

#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_UUID_SIZE_EXT 16
#define GL_NUM_DEVICE_UUIDS_EXT 0x9596
#define GL_DEVICE_UUID_EXT 0x9597
#define GL_DRIVER_UUID_EXT 0x9598
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned char GLubyte;
typedef int64_t GLint64;
typedef const GLubyte* (GLAPIENTRY * PFNGLGETSTRINGPROC) (GLenum name);
typedef const GLubyte* (GLAPIENTRY * PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETINTEGERVPROC) (GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETUNSIGNEDBYTEVEXTPROC) (GLenum pname, GLubyte* data);
typedef void (GLAPIENTRY * PFNGLGETUNSIGNEDBYTEI_VEXTPROC) (GLenum target, GLuint index, GLubyte* data);

#ifndef TOSTRING
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#endif

#ifdef _WIN32
#define dlsym GetProcAddress
#endif

#endif //QUERYVKCOOPMAT_GLCOMMON_HPP
