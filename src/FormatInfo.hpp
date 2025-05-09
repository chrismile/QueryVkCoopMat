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

#ifndef FORMATINFO_HPP
#define FORMATINFO_HPP

#include <cstdint>
#include "drm_fourcc.h"

inline std::string convertVkFormatToString(VkFormat format) {
    switch(format) {
    case VK_FORMAT_UNDEFINED:
       return "VK_FORMAT_UNDEFINED";
    case VK_FORMAT_R4G4_UNORM_PACK8:
       return "VK_FORMAT_R4G4_UNORM_PACK8";
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
       return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
       return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    case VK_FORMAT_R5G6B5_UNORM_PACK16:
       return "VK_FORMAT_R5G6B5_UNORM_PACK16";
    case VK_FORMAT_B5G6R5_UNORM_PACK16:
       return "VK_FORMAT_B5G6R5_UNORM_PACK16";
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
       return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
       return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
       return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    case VK_FORMAT_R8_UNORM:
       return "VK_FORMAT_R8_UNORM";
    case VK_FORMAT_R8_SNORM:
       return "VK_FORMAT_R8_SNORM";
    case VK_FORMAT_R8_USCALED:
       return "VK_FORMAT_R8_USCALED";
    case VK_FORMAT_R8_SSCALED:
       return "VK_FORMAT_R8_SSCALED";
    case VK_FORMAT_R8_UINT:
       return "VK_FORMAT_R8_UINT";
    case VK_FORMAT_R8_SINT:
       return "VK_FORMAT_R8_SINT";
    case VK_FORMAT_R8_SRGB:
       return "VK_FORMAT_R8_SRGB";
    case VK_FORMAT_R8G8_UNORM:
       return "VK_FORMAT_R8G8_UNORM";
    case VK_FORMAT_R8G8_SNORM:
       return "VK_FORMAT_R8G8_SNORM";
    case VK_FORMAT_R8G8_USCALED:
       return "VK_FORMAT_R8G8_USCALED";
    case VK_FORMAT_R8G8_SSCALED:
       return "VK_FORMAT_R8G8_SSCALED";
    case VK_FORMAT_R8G8_UINT:
       return "VK_FORMAT_R8G8_UINT";
    case VK_FORMAT_R8G8_SINT:
       return "VK_FORMAT_R8G8_SINT";
    case VK_FORMAT_R8G8_SRGB:
       return "VK_FORMAT_R8G8_SRGB";
    case VK_FORMAT_R8G8B8_UNORM:
       return "VK_FORMAT_R8G8B8_UNORM";
    case VK_FORMAT_R8G8B8_SNORM:
       return "VK_FORMAT_R8G8B8_SNORM";
    case VK_FORMAT_R8G8B8_USCALED:
       return "VK_FORMAT_R8G8B8_USCALED";
    case VK_FORMAT_R8G8B8_SSCALED:
       return "VK_FORMAT_R8G8B8_SSCALED";
    case VK_FORMAT_R8G8B8_UINT:
       return "VK_FORMAT_R8G8B8_UINT";
    case VK_FORMAT_R8G8B8_SINT:
       return "VK_FORMAT_R8G8B8_SINT";
    case VK_FORMAT_R8G8B8_SRGB:
       return "VK_FORMAT_R8G8B8_SRGB";
    case VK_FORMAT_B8G8R8_UNORM:
       return "VK_FORMAT_B8G8R8_UNORM";
    case VK_FORMAT_B8G8R8_SNORM:
       return "VK_FORMAT_B8G8R8_SNORM";
    case VK_FORMAT_B8G8R8_USCALED:
       return "VK_FORMAT_B8G8R8_USCALED";
    case VK_FORMAT_B8G8R8_SSCALED:
       return "VK_FORMAT_B8G8R8_SSCALED";
    case VK_FORMAT_B8G8R8_UINT:
       return "VK_FORMAT_B8G8R8_UINT";
    case VK_FORMAT_B8G8R8_SINT:
       return "VK_FORMAT_B8G8R8_SINT";
    case VK_FORMAT_B8G8R8_SRGB:
       return "VK_FORMAT_B8G8R8_SRGB";
    case VK_FORMAT_R8G8B8A8_UNORM:
       return "VK_FORMAT_R8G8B8A8_UNORM";
    case VK_FORMAT_R8G8B8A8_SNORM:
       return "VK_FORMAT_R8G8B8A8_SNORM";
    case VK_FORMAT_R8G8B8A8_USCALED:
       return "VK_FORMAT_R8G8B8A8_USCALED";
    case VK_FORMAT_R8G8B8A8_SSCALED:
       return "VK_FORMAT_R8G8B8A8_SSCALED";
    case VK_FORMAT_R8G8B8A8_UINT:
       return "VK_FORMAT_R8G8B8A8_UINT";
    case VK_FORMAT_R8G8B8A8_SINT:
       return "VK_FORMAT_R8G8B8A8_SINT";
    case VK_FORMAT_R8G8B8A8_SRGB:
       return "VK_FORMAT_R8G8B8A8_SRGB";
    case VK_FORMAT_B8G8R8A8_UNORM:
       return "VK_FORMAT_B8G8R8A8_UNORM";
    case VK_FORMAT_B8G8R8A8_SNORM:
       return "VK_FORMAT_B8G8R8A8_SNORM";
    case VK_FORMAT_B8G8R8A8_USCALED:
       return "VK_FORMAT_B8G8R8A8_USCALED";
    case VK_FORMAT_B8G8R8A8_SSCALED:
       return "VK_FORMAT_B8G8R8A8_SSCALED";
    case VK_FORMAT_B8G8R8A8_UINT:
       return "VK_FORMAT_B8G8R8A8_UINT";
    case VK_FORMAT_B8G8R8A8_SINT:
       return "VK_FORMAT_B8G8R8A8_SINT";
    case VK_FORMAT_B8G8R8A8_SRGB:
       return "VK_FORMAT_B8G8R8A8_SRGB";
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
       return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
       return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
       return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
       return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_UINT_PACK32:
       return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SINT_PACK32:
       return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
       return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
       return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
       return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
       return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
       return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_UINT_PACK32:
       return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
    case VK_FORMAT_A2R10G10B10_SINT_PACK32:
       return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
       return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
       return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
       return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
       return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_UINT_PACK32:
       return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
    case VK_FORMAT_A2B10G10R10_SINT_PACK32:
       return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
    case VK_FORMAT_R16_UNORM:
       return "VK_FORMAT_R16_UNORM";
    case VK_FORMAT_R16_SNORM:
       return "VK_FORMAT_R16_SNORM";
    case VK_FORMAT_R16_USCALED:
       return "VK_FORMAT_R16_USCALED";
    case VK_FORMAT_R16_SSCALED:
       return "VK_FORMAT_R16_SSCALED";
    case VK_FORMAT_R16_UINT:
       return "VK_FORMAT_R16_UINT";
    case VK_FORMAT_R16_SINT:
       return "VK_FORMAT_R16_SINT";
    case VK_FORMAT_R16_SFLOAT:
       return "VK_FORMAT_R16_SFLOAT";
    case VK_FORMAT_R16G16_UNORM:
       return "VK_FORMAT_R16G16_UNORM";
    case VK_FORMAT_R16G16_SNORM:
       return "VK_FORMAT_R16G16_SNORM";
    case VK_FORMAT_R16G16_USCALED:
       return "VK_FORMAT_R16G16_USCALED";
    case VK_FORMAT_R16G16_SSCALED:
       return "VK_FORMAT_R16G16_SSCALED";
    case VK_FORMAT_R16G16_UINT:
       return "VK_FORMAT_R16G16_UINT";
    case VK_FORMAT_R16G16_SINT:
       return "VK_FORMAT_R16G16_SINT";
    case VK_FORMAT_R16G16_SFLOAT:
       return "VK_FORMAT_R16G16_SFLOAT";
    case VK_FORMAT_R16G16B16_UNORM:
       return "VK_FORMAT_R16G16B16_UNORM";
    case VK_FORMAT_R16G16B16_SNORM:
       return "VK_FORMAT_R16G16B16_SNORM";
    case VK_FORMAT_R16G16B16_USCALED:
       return "VK_FORMAT_R16G16B16_USCALED";
    case VK_FORMAT_R16G16B16_SSCALED:
       return "VK_FORMAT_R16G16B16_SSCALED";
    case VK_FORMAT_R16G16B16_UINT:
       return "VK_FORMAT_R16G16B16_UINT";
    case VK_FORMAT_R16G16B16_SINT:
       return "VK_FORMAT_R16G16B16_SINT";
    case VK_FORMAT_R16G16B16_SFLOAT:
       return "VK_FORMAT_R16G16B16_SFLOAT";
    case VK_FORMAT_R16G16B16A16_UNORM:
       return "VK_FORMAT_R16G16B16A16_UNORM";
    case VK_FORMAT_R16G16B16A16_SNORM:
       return "VK_FORMAT_R16G16B16A16_SNORM";
    case VK_FORMAT_R16G16B16A16_USCALED:
       return "VK_FORMAT_R16G16B16A16_USCALED";
    case VK_FORMAT_R16G16B16A16_SSCALED:
       return "VK_FORMAT_R16G16B16A16_SSCALED";
    case VK_FORMAT_R16G16B16A16_UINT:
       return "VK_FORMAT_R16G16B16A16_UINT";
    case VK_FORMAT_R16G16B16A16_SINT:
       return "VK_FORMAT_R16G16B16A16_SINT";
    case VK_FORMAT_R16G16B16A16_SFLOAT:
       return "VK_FORMAT_R16G16B16A16_SFLOAT";
    case VK_FORMAT_R32_UINT:
       return "VK_FORMAT_R32_UINT";
    case VK_FORMAT_R32_SINT:
       return "VK_FORMAT_R32_SINT";
    case VK_FORMAT_R32_SFLOAT:
       return "VK_FORMAT_R32_SFLOAT";
    case VK_FORMAT_R32G32_UINT:
       return "VK_FORMAT_R32G32_UINT";
    case VK_FORMAT_R32G32_SINT:
       return "VK_FORMAT_R32G32_SINT";
    case VK_FORMAT_R32G32_SFLOAT:
       return "VK_FORMAT_R32G32_SFLOAT";
    case VK_FORMAT_R32G32B32_UINT:
       return "VK_FORMAT_R32G32B32_UINT";
    case VK_FORMAT_R32G32B32_SINT:
       return "VK_FORMAT_R32G32B32_SINT";
    case VK_FORMAT_R32G32B32_SFLOAT:
       return "VK_FORMAT_R32G32B32_SFLOAT";
    case VK_FORMAT_R32G32B32A32_UINT:
       return "VK_FORMAT_R32G32B32A32_UINT";
    case VK_FORMAT_R32G32B32A32_SINT:
       return "VK_FORMAT_R32G32B32A32_SINT";
    case VK_FORMAT_R32G32B32A32_SFLOAT:
       return "VK_FORMAT_R32G32B32A32_SFLOAT";
    case VK_FORMAT_R64_UINT:
       return "VK_FORMAT_R64_UINT";
    case VK_FORMAT_R64_SINT:
       return "VK_FORMAT_R64_SINT";
    case VK_FORMAT_R64_SFLOAT:
       return "VK_FORMAT_R64_SFLOAT";
    case VK_FORMAT_R64G64_UINT:
       return "VK_FORMAT_R64G64_UINT";
    case VK_FORMAT_R64G64_SINT:
       return "VK_FORMAT_R64G64_SINT";
    case VK_FORMAT_R64G64_SFLOAT:
       return "VK_FORMAT_R64G64_SFLOAT";
    case VK_FORMAT_R64G64B64_UINT:
       return "VK_FORMAT_R64G64B64_UINT";
    case VK_FORMAT_R64G64B64_SINT:
       return "VK_FORMAT_R64G64B64_SINT";
    case VK_FORMAT_R64G64B64_SFLOAT:
       return "VK_FORMAT_R64G64B64_SFLOAT";
    case VK_FORMAT_R64G64B64A64_UINT:
       return "VK_FORMAT_R64G64B64A64_UINT";
    case VK_FORMAT_R64G64B64A64_SINT:
       return "VK_FORMAT_R64G64B64A64_SINT";
    case VK_FORMAT_R64G64B64A64_SFLOAT:
       return "VK_FORMAT_R64G64B64A64_SFLOAT";
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
       return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
       return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
    case VK_FORMAT_D16_UNORM:
       return "VK_FORMAT_D16_UNORM";
    case VK_FORMAT_X8_D24_UNORM_PACK32:
       return "VK_FORMAT_X8_D24_UNORM_PACK32";
    case VK_FORMAT_D32_SFLOAT:
       return "VK_FORMAT_D32_SFLOAT";
    case VK_FORMAT_S8_UINT:
       return "VK_FORMAT_S8_UINT";
    case VK_FORMAT_D16_UNORM_S8_UINT:
       return "VK_FORMAT_D16_UNORM_S8_UINT";
    case VK_FORMAT_D24_UNORM_S8_UINT:
       return "VK_FORMAT_D24_UNORM_S8_UINT";
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
       return "VK_FORMAT_D32_SFLOAT_S8_UINT";
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
       return "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
       return "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
       return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
       return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
    case VK_FORMAT_BC2_UNORM_BLOCK:
       return "VK_FORMAT_BC2_UNORM_BLOCK";
    case VK_FORMAT_BC2_SRGB_BLOCK:
       return "VK_FORMAT_BC2_SRGB_BLOCK";
    case VK_FORMAT_BC3_UNORM_BLOCK:
       return "VK_FORMAT_BC3_UNORM_BLOCK";
    case VK_FORMAT_BC3_SRGB_BLOCK:
       return "VK_FORMAT_BC3_SRGB_BLOCK";
    case VK_FORMAT_BC4_UNORM_BLOCK:
       return "VK_FORMAT_BC4_UNORM_BLOCK";
    case VK_FORMAT_BC4_SNORM_BLOCK:
       return "VK_FORMAT_BC4_SNORM_BLOCK";
    case VK_FORMAT_BC5_UNORM_BLOCK:
       return "VK_FORMAT_BC5_UNORM_BLOCK";
    case VK_FORMAT_BC5_SNORM_BLOCK:
       return "VK_FORMAT_BC5_SNORM_BLOCK";
    case VK_FORMAT_BC6H_UFLOAT_BLOCK:
       return "VK_FORMAT_BC6H_UFLOAT_BLOCK";
    case VK_FORMAT_BC6H_SFLOAT_BLOCK:
       return "VK_FORMAT_BC6H_SFLOAT_BLOCK";
    case VK_FORMAT_BC7_UNORM_BLOCK:
       return "VK_FORMAT_BC7_UNORM_BLOCK";
    case VK_FORMAT_BC7_SRGB_BLOCK:
       return "VK_FORMAT_BC7_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
       return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
    case VK_FORMAT_EAC_R11_UNORM_BLOCK:
       return "VK_FORMAT_EAC_R11_UNORM_BLOCK";
    case VK_FORMAT_EAC_R11_SNORM_BLOCK:
       return "VK_FORMAT_EAC_R11_SNORM_BLOCK";
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
       return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
       return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
       return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
       return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
    case VK_FORMAT_G8B8G8R8_422_UNORM:
       return "VK_FORMAT_G8B8G8R8_422_UNORM";
    case VK_FORMAT_B8G8R8G8_422_UNORM:
       return "VK_FORMAT_B8G8R8G8_422_UNORM";
    case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
       return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM";
    case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
       return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM";
    case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
       return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM";
    case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
       return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM";
    case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
       return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM";
    case VK_FORMAT_R10X6_UNORM_PACK16:
       return "VK_FORMAT_R10X6_UNORM_PACK16";
    case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
       return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16";
    case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
       return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16";
    case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
       return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16";
    case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
       return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16";
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16";
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16";
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16";
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16";
    case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16";
    case VK_FORMAT_R12X4_UNORM_PACK16:
       return "VK_FORMAT_R12X4_UNORM_PACK16";
    case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
       return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16";
    case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
       return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16";
    case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
       return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16";
    case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
       return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16";
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16";
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16";
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16";
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16";
    case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16";
    case VK_FORMAT_G16B16G16R16_422_UNORM:
       return "VK_FORMAT_G16B16G16R16_422_UNORM";
    case VK_FORMAT_B16G16R16G16_422_UNORM:
       return "VK_FORMAT_B16G16R16G16_422_UNORM";
    case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
       return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM";
    case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
       return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM";
    case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
       return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM";
    case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
       return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM";
    case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
       return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM";
    case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:
       return "VK_FORMAT_G8_B8R8_2PLANE_444_UNORM";
    case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:
       return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16";
    case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:
       return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16";
    case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:
       return "VK_FORMAT_G16_B16R16_2PLANE_444_UNORM";
    case VK_FORMAT_A4R4G4B4_UNORM_PACK16:
       return "VK_FORMAT_A4R4G4B4_UNORM_PACK16";
    case VK_FORMAT_A4B4G4R4_UNORM_PACK16:
       return "VK_FORMAT_A4B4G4R4_UNORM_PACK16";
    case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK";
    case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:
       return "VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK";
    case VK_FORMAT_A1B5G5R5_UNORM_PACK16:
       return "VK_FORMAT_A1B5G5R5_UNORM_PACK16";
    case VK_FORMAT_A8_UNORM:
       return "VK_FORMAT_A8_UNORM";
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
       return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
       return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
       return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
       return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
       return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
       return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
       return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
       return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_R16G16_SFIXED5_NV:
       return "VK_FORMAT_R16G16_SFIXED5_NV";
    default:
        return "UNKNOWN";
    }
}

std::string convertVkFormatFeatureFlagsToString(VkFormatFeatureFlags flags) {
    std::vector<std::string> featureFlagNames;
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE");
    }
    if ((flags & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_STORAGE_IMAGE");
    }
    if ((flags & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_STORAGE_IMAGE_ATOMIC");
    }
    if ((flags & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_UNIFORM_TEXEL_BUFFER");
    }
    if ((flags & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_STORAGE_TEXEL_BUFFER");
    }
    if ((flags & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC");
    }
    if ((flags & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_VERTEX_BUFFER");
    }
    if ((flags & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_COLOR_ATTACHMENT");
    }
    if ((flags & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_COLOR_ATTACHMENT_BLEND");
    }
    if ((flags & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_DEPTH_STENCIL_ATTACHMENT");
    }
    if ((flags & VK_FORMAT_FEATURE_BLIT_SRC_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_BLIT_SRC");
    }
    if ((flags & VK_FORMAT_FEATURE_BLIT_DST_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_BLIT_DST");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_FILTER_LINEAR");
    }
    if ((flags & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_TRANSFER_SRC");
    }
    if ((flags & VK_FORMAT_FEATURE_TRANSFER_DST_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_TRANSFER_DST");
    }
    if ((flags & VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_MIDPOINT_CHROMA_SAMPLES");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE");
    }
    if ((flags & VK_FORMAT_FEATURE_DISJOINT_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_DISJOINT");
    }
    if ((flags & VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_COSITED_CHROMA_SAMPLES");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_FILTER_MINMAX");
    }
    if ((flags & VK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_VIDEO_DECODE_OUTPUT");
    }
    if ((flags & VK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_VIDEO_DECODE_DPB");
    }
    if ((flags & VK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_FILTER_CUBIC");
    }
    if ((flags & VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT) != 0) {
        featureFlagNames.emplace_back("FEATURE_FRAGMENT_DENSITY_MAP");
    }
    if ((flags & VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT");
    }
    if ((flags & VK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_VIDEO_ENCODE_INPUT");
    }
    if ((flags & VK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_VIDEO_ENCODE_DPB");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_FILTER_CUBIC");
    }
    if ((flags & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_TRANSFER_SRC");
    }
    if ((flags & VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_TRANSFER_DST");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT_EXT) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_FILTER_MINMAX");
    }
    if ((flags & VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_MIDPOINT_CHROMA_SAMPLES");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT");
    }
    if ((flags & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE");
    }
    if ((flags & VK_FORMAT_FEATURE_DISJOINT_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_DISJOINT");
    }
    if ((flags & VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT_KHR) != 0) {
        featureFlagNames.emplace_back("FEATURE_COSITED_CHROMA_SAMPLES");
    }
    std::string featureFlagsString;
    for (size_t i = 0; i < featureFlagNames.size(); i++) {
        if (i != 0) {
            featureFlagsString += " | ";
        }
        featureFlagsString += featureFlagNames.at(i);
    }
    return featureFlagsString;
}

std::string convertDrmVendorIdToString(uint64_t vendorId) {
    switch(vendorId) {
    case DRM_FORMAT_MOD_VENDOR_NONE:
        return "NONE";
    case DRM_FORMAT_MOD_VENDOR_INTEL:
        return "INTEL";
    case DRM_FORMAT_MOD_VENDOR_AMD:
        return "AMD";
    case DRM_FORMAT_MOD_VENDOR_NVIDIA:
        return "NVIDIA";
    case DRM_FORMAT_MOD_VENDOR_SAMSUNG:
        return "SAMSUNG";
    case DRM_FORMAT_MOD_VENDOR_QCOM:
        return "QCOM";
    case DRM_FORMAT_MOD_VENDOR_VIVANTE:
        return "VIVANTE";
    case DRM_FORMAT_MOD_VENDOR_BROADCOM:
        return "BROADCOM";
    case DRM_FORMAT_MOD_VENDOR_ARM:
        return "ARM";
    case DRM_FORMAT_MOD_VENDOR_ALLWINNER:
        return "ALLWINNER";
    case DRM_FORMAT_MOD_VENDOR_AMLOGIC:
        return "AMLOGIC";
    case DRM_FORMAT_MOD_VENDOR_MTK:
        return "MTK";
    default:
        return "UNKNOWN";
    }
}

/*
 * Uses information from:
 * https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/plain/include/uapi/drm/drm_fourcc.h
 */
inline std::string convertDrmFormatModifierToString(uint64_t formatModifier) {
    if (formatModifier == DRM_FORMAT_MOD_INVALID) {
        return "INVALID";
    }
    if (formatModifier == DRM_FORMAT_MOD_LINEAR) {
        return "LINEAR";
    }
    auto vendorId = fourcc_mod_get_vendor(formatModifier);
    if (vendorId == DRM_FORMAT_MOD_VENDOR_INTEL) {
        switch (formatModifier) {
        case I915_FORMAT_MOD_X_TILED:
            return "I915_FORMAT_MOD_X_TILED";
        case I915_FORMAT_MOD_Y_TILED:
            return "I915_FORMAT_MOD_Y_TILED";
        case I915_FORMAT_MOD_Yf_TILED:
            return "I915_FORMAT_MOD_Yf_TILED";
        case I915_FORMAT_MOD_Y_TILED_CCS:
            return "I915_FORMAT_MOD_Y_TILED_CCS";
        case I915_FORMAT_MOD_Yf_TILED_CCS:
            return "I915_FORMAT_MOD_Yf_TILED_CCS";
        case I915_FORMAT_MOD_Y_TILED_GEN12_RC_CCS:
            return "I915_FORMAT_MOD_Y_TILED_GEN12_RC_CCS";
        case I915_FORMAT_MOD_Y_TILED_GEN12_MC_CCS:
            return "I915_FORMAT_MOD_Y_TILED_GEN12_MC_CCS";
        case I915_FORMAT_MOD_Y_TILED_GEN12_RC_CCS_CC:
            return "I915_FORMAT_MOD_Y_TILED_GEN12_RC_CCS_CC";
        case I915_FORMAT_MOD_4_TILED:
            return "I915_FORMAT_MOD_4_TILED";
        case I915_FORMAT_MOD_4_TILED_DG2_RC_CCS:
            return "I915_FORMAT_MOD_4_TILED_DG2_RC_CCS";
        case I915_FORMAT_MOD_4_TILED_DG2_MC_CCS:
            return "I915_FORMAT_MOD_4_TILED_DG2_MC_CCS";
        case I915_FORMAT_MOD_4_TILED_DG2_RC_CCS_CC:
            return "I915_FORMAT_MOD_4_TILED_DG2_RC_CCS_CC";
        case I915_FORMAT_MOD_4_TILED_MTL_RC_CCS:
            return "I915_FORMAT_MOD_4_TILED_MTL_RC_CCS";
        case I915_FORMAT_MOD_4_TILED_MTL_MC_CCS:
            return "I915_FORMAT_MOD_4_TILED_MTL_MC_CCS";
        case I915_FORMAT_MOD_4_TILED_MTL_RC_CCS_CC:
            return "I915_FORMAT_MOD_4_TILED_MTL_RC_CCS_CC";
        case I915_FORMAT_MOD_4_TILED_LNL_CCS:
            return "I915_FORMAT_MOD_4_TILED_LNL_CCS";
        case I915_FORMAT_MOD_4_TILED_BMG_CCS:
            return "I915_FORMAT_MOD_4_TILED_BMG_CCS";
        }
    }
    auto vendorName = convertDrmVendorIdToString(vendorId);
    return sgl::toHexString(formatModifier) + " (" + vendorName + ")";
}

#endif //FORMATINFO_HPP
