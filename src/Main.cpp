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

#include <iostream>
#include <utility>

#include <Utils/File/Logfile.hpp>
#include <Graphics/Vulkan/Utils/Instance.hpp>
#include <Graphics/Vulkan/Utils/Device.hpp>
#include <ImGui/Widgets/NumberFormatting.hpp>

#include "Math/Math.hpp"

#ifdef __linux__
#include <fstream>
#include "OffscreenContextEGL.hpp"
#include "FormatInfo.hpp"
#endif

#ifdef _WIN32
#include "OffscreenContextWGL.hpp"
#include "WindowsUtils.hpp"
#endif

#define RES_TO_STR(r) case r: return #r

std::string getComponentTypeString(VkComponentTypeKHR compType) {
    switch (compType) {
        case VK_COMPONENT_TYPE_FLOAT16_KHR:
            return "float16";
        case VK_COMPONENT_TYPE_FLOAT32_KHR:
            return "float32";
        case VK_COMPONENT_TYPE_FLOAT64_KHR:
            return "float64";
        case VK_COMPONENT_TYPE_SINT8_KHR:
            return "sint8";
        case VK_COMPONENT_TYPE_SINT16_KHR:
            return "sint16";
        case VK_COMPONENT_TYPE_SINT32_KHR:
            return "sint32";
        case VK_COMPONENT_TYPE_SINT64_KHR:
            return "sint64";
        case VK_COMPONENT_TYPE_UINT8_KHR:
            return "uint8";
        case VK_COMPONENT_TYPE_UINT16_KHR:
            return "uint16";
        case VK_COMPONENT_TYPE_UINT32_KHR:
            return "uint32";
        case VK_COMPONENT_TYPE_UINT64_KHR:
            return "uint64";
        case VK_COMPONENT_TYPE_BFLOAT16_KHR:
            return "bloat16";
        case VK_COMPONENT_TYPE_SINT8_PACKED_NV:
            return "sint8_packed";
        case VK_COMPONENT_TYPE_UINT8_PACKED_NV:
            return "uint8_packed";
        case VK_COMPONENT_TYPE_FLOAT_E4M3_NV:
            return "float_e4m3";
        case VK_COMPONENT_TYPE_FLOAT_E5M2_NV:
            return "float_e5m2";
        default:
            return "UNKNOWN";
    }
}

std::string getScopeString(VkScopeKHR scope) {
    switch (scope) {
        case VK_SCOPE_DEVICE_KHR:
            return "DEVICE";
        case VK_SCOPE_WORKGROUP_KHR:
            return "WORKGROUP";
        case VK_SCOPE_SUBGROUP_KHR:
            return "SUBGROUP";
        case VK_SCOPE_QUEUE_FAMILY_KHR:
            return "QUEUE_FAMILY";
        default:
            return "UNKNOWN";
    }
}

std::string shaderStagesToString(VkShaderStageFlags stageFlags) {
    std::vector<std::string> shaderStageNames;
    if ((stageFlags & VK_SHADER_STAGE_VERTEX_BIT) != 0) {
        shaderStageNames.emplace_back("VERTEX");
    }
    if ((stageFlags & VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT) != 0) {
        shaderStageNames.emplace_back("TESSELLATION_CONTROL");
    }
    if ((stageFlags & VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT) != 0) {
        shaderStageNames.emplace_back("TESSELLATION_EVALUATION");
    }
    if ((stageFlags & VK_SHADER_STAGE_GEOMETRY_BIT) != 0) {
        shaderStageNames.emplace_back("GEOMETRY");
    }
    if ((stageFlags & VK_SHADER_STAGE_FRAGMENT_BIT) != 0) {
        shaderStageNames.emplace_back("FRAGMENT");
    }
    if ((stageFlags & VK_SHADER_STAGE_COMPUTE_BIT) != 0) {
        shaderStageNames.emplace_back("COMPUTE");
    }
    if ((stageFlags & VK_SHADER_STAGE_RAYGEN_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("RAYGEN");
    }
    if ((stageFlags & VK_SHADER_STAGE_ANY_HIT_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("ANY_HIT");
    }
    if ((stageFlags & VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("CLOSEST_HIT");
    }
    if ((stageFlags & VK_SHADER_STAGE_MISS_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("MISS");
    }
    if ((stageFlags & VK_SHADER_STAGE_INTERSECTION_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("INTERSECTION");
    }
    if ((stageFlags & VK_SHADER_STAGE_CALLABLE_BIT_KHR) != 0) {
        shaderStageNames.emplace_back("CALLABLE");
    }
    if ((stageFlags & VK_SHADER_STAGE_TASK_BIT_EXT) != 0) {
        shaderStageNames.emplace_back("TASK");
    }
    if ((stageFlags & VK_SHADER_STAGE_MESH_BIT_EXT) != 0) {
        shaderStageNames.emplace_back("MESH");
    }
    std::string shaderStages;
    for (size_t i = 0; i < shaderStageNames.size(); i++) {
        if (i != 0) {
            shaderStages += " | ";
        }
        shaderStages += shaderStageNames.at(i);
    }
    return shaderStages;
}

namespace sgl {
// Override for nicer formating of bools.
inline std::string toString(bool boolVal) {
    return boolVal ? "true" : "false";
}
}

template<typename... T>
void writeOut(T... args) {
    std::string text = (std::string() + ... + sgl::toString(std::move(args)));
    sgl::Logfile::get()->write(text, sgl::BLACK);
    std::cout << text << std::endl;
}

std::string uint8ArrayToHex(const uint8_t* arr, size_t numEntries) {
    std::string hexRep;
    for (size_t i = 0; i < numEntries; i++) {
        hexRep += sgl::toHexString(uint32_t(arr[i]));
    }
    return hexRep;
}

void checkCooperativeMatrixFeaturesKHR(sgl::vk::Device* device) {
    if (!device->getCooperativeMatrixFeaturesKHR().cooperativeMatrix) {
        writeOut("");
        writeOut("VK_KHR_cooperative_matrix is not supported.");
        return;
    }

    const auto& cooperativeMatrixProperties = device->getSupportedCooperativeMatrixPropertiesKHR();

    writeOut("");
    writeOut("VK_KHR_cooperative_matrix properties:");
    writeOut("");
    sgl::Logfile::get()->write("<table><tr><th>MSize</th><th>NSize</th><th>KSize</th><th>AType</th><th>BType</th><th>CType</th><th>ResultType</th><th>sat</th><th>scope</th></tr>\n");
    for (size_t i = 0; i < cooperativeMatrixProperties.size(); i++) {
        auto& props = cooperativeMatrixProperties[i];
        std::cout
                << "MSize: " << props.MSize
                << "\nNSize: " << props.NSize
                << "\nKSize: " << props.KSize
                << "\nAType: " << getComponentTypeString(props.AType)
                << "\nBType: " << getComponentTypeString(props.BType)
                << "\nCType: " << getComponentTypeString(props.CType)
                << "\nResultType: " << getComponentTypeString(props.ResultType)
                << "\nsaturatingAccumulation: " << sgl::toString(bool(props.saturatingAccumulation))
                << "\nscope: " << getScopeString(props.scope)
                << "\n" << std::endl;
        sgl::Logfile::get()->write("<tr>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.MSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.NSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.KSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.AType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.BType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.CType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.ResultType) +"</td>");
        sgl::Logfile::get()->write("<td>" + sgl::toString(bool(props.saturatingAccumulation)) +"</td>");
        sgl::Logfile::get()->write("<td>" + getScopeString(props.scope) +"</td>");
        sgl::Logfile::get()->write("</tr>\n");
    }
    sgl::Logfile::get()->write("</table>\n");
}

void checkCooperativeMatrixFeaturesNV2(sgl::vk::Device* device) {
    if (!device->isDeviceExtensionSupported(VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME)) {
        writeOut("");
        writeOut("VK_NV_cooperative_matrix2 is not supported.");
        return;
    }

    const auto& features = device->getCooperativeMatrix2FeaturesNV();
    const auto& properties = device->getCooperativeMatrix2PropertiesNV();
    const auto& flexibleDimensionsProperties = device->getSupportedCooperativeMatrixFlexibleDimensionsPropertiesNV();
    writeOut("");
    writeOut("VK_NV_cooperative_matrix2 properties:");
    writeOut("cooperativeMatrixWorkgroupScope: ", bool(features.cooperativeMatrixWorkgroupScope));
    writeOut("cooperativeMatrixFlexibleDimensions: ", bool(features.cooperativeMatrixFlexibleDimensions));
    writeOut("cooperativeMatrixReductions: ", bool(features.cooperativeMatrixReductions));
    writeOut("cooperativeMatrixConversions: ", bool(features.cooperativeMatrixConversions));
    writeOut("cooperativeMatrixPerElementOperations: ", bool(features.cooperativeMatrixPerElementOperations));
    writeOut("cooperativeMatrixTensorAddressing: ", bool(features.cooperativeMatrixTensorAddressing));
    writeOut("cooperativeMatrixBlockLoads: ", bool(features.cooperativeMatrixBlockLoads));
    writeOut("cooperativeMatrixWorkgroupScopeMaxWorkgroupSize: ", properties.cooperativeMatrixWorkgroupScopeMaxWorkgroupSize);
    writeOut("cooperativeMatrixFlexibleDimensionsMaxDimension: ", properties.cooperativeMatrixFlexibleDimensionsMaxDimension);
    writeOut("cooperativeMatrixWorkgroupScopeReservedSharedMemory: ", properties.cooperativeMatrixWorkgroupScopeReservedSharedMemory);
    writeOut("");
    sgl::Logfile::get()->write("<table><tr><th>MGranularity</th><th>NGranularity</th><th>KGranularity</th><th>AType</th><th>BType</th><th>CType</th><th>ResultType</th><th>sat</th><th>scope</th><th>WGInvocs</th></tr>\n");
    for (size_t i = 0; i < flexibleDimensionsProperties.size(); i++) {
        auto& props = flexibleDimensionsProperties[i];
        std::cout
                << "MGranularity: " << props.MGranularity
                << "\nNGranularity: " << props.NGranularity
                << "\nKGranularity: " << props.KGranularity
                << "\nAType: " << getComponentTypeString(props.AType)
                << "\nBType: " << getComponentTypeString(props.BType)
                << "\nCType: " << getComponentTypeString(props.CType)
                << "\nResultType: " << getComponentTypeString(props.ResultType)
                << "\nsaturatingAccumulation: " << sgl::toString(bool(props.saturatingAccumulation))
                << "\nscope: " << getScopeString(props.scope)
                << "\nworkgroupInvocations: " << props.workgroupInvocations
                << "\n" << std::endl;
        sgl::Logfile::get()->write("<tr>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.MGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.NGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.KGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.AType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.BType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.CType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.ResultType) +"</td>");
        sgl::Logfile::get()->write("<td>" + sgl::toString(bool(props.saturatingAccumulation)) +"</td>");
        sgl::Logfile::get()->write("<td>" + getScopeString(props.scope) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.workgroupInvocations) +"</td>");
        sgl::Logfile::get()->write("</tr>\n");
    }
    sgl::Logfile::get()->write("</table>\n");
}

void checkCooperativeVectorFeaturesNV(sgl::vk::Device* device) {
    if (!device->isDeviceExtensionSupported(VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME)) {
        writeOut("");
        writeOut("VK_NV_cooperative_vector is not supported.");
        return;
    }

    const auto& features = device->getCooperativeVectorFeaturesNV();
    const auto& properties = device->getCooperativeVectorPropertiesNV();
    const auto& supportedProperties = device->getSupportedCooperativeVectorPropertiesNV();
    writeOut("");
    writeOut("VK_NV_cooperative_vector properties:");
    writeOut("cooperativeVector: ", bool(features.cooperativeVector));
    writeOut("cooperativeVectorTraining: ", bool(features.cooperativeVectorTraining));
    writeOut("cooperativeVectorSupportedStages: ", shaderStagesToString(properties.cooperativeVectorSupportedStages));
    writeOut("cooperativeVectorTrainingFloat16Accumulation: ", properties.cooperativeVectorTrainingFloat16Accumulation);
    writeOut("cooperativeVectorTrainingFloat32Accumulation: ", properties.cooperativeVectorTrainingFloat32Accumulation);
    writeOut("maxCooperativeVectorComponents: ", properties.maxCooperativeVectorComponents);
    writeOut("");
    sgl::Logfile::get()->write("<table><tr><th>inputType</th><th>inputInterpretation</th><th>matrixInterpretation</th><th>biasInterpretation</th><th>resultType</th><th>transpose</th></tr>\n");
    for (size_t i = 0; i < supportedProperties.size(); i++) {
        auto& props = supportedProperties[i];
        std::cout
                << "inputType: " << getComponentTypeString(props.inputType)
                << "\ninputInterpretation: " << getComponentTypeString(props.inputInterpretation)
                << "\nmatrixInterpretation: " << getComponentTypeString(props.matrixInterpretation)
                << "\nbiasInterpretation: " << getComponentTypeString(props.biasInterpretation)
                << "\nresultType: " << getComponentTypeString(props.resultType)
                << "\ntranspose: " << sgl::toString(bool(props.transpose))
                << "\n" << std::endl;
        sgl::Logfile::get()->write("<tr>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.inputType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.inputInterpretation) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.matrixInterpretation) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.biasInterpretation) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.resultType) +"</td>");
        sgl::Logfile::get()->write("<td>" + sgl::toString(bool(props.transpose)) + "</td>");
        sgl::Logfile::get()->write("</tr>\n");
    }
    sgl::Logfile::get()->write("</table>\n");
}

void checkCooperativeMatrixFeatures(sgl::vk::Device* device) {
    sgl::Logfile::get()->write("<br>");
    writeOut(std::string() + "Device name: " + device->getDeviceName());
    if (device->getPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1) {
        writeOut("Device driver name: ", device->getDeviceDriverName());
        writeOut("Device driver info: ", device->getDeviceDriverInfo());
        writeOut("Device driver ID: ", device->getDeviceDriverId());
        writeOut("Device driver version: ", device->getDriverVersionString());
        //writeOut("Device driver UUID: ", uint8ArrayToHex(device->getDeviceIDProperties().deviceUUID, VK_UUID_SIZE));
        //writeOut("Device driver LUID: ", uint8ArrayToHex(device->getDeviceIDProperties().deviceLUID, VK_UUID_SIZE));
    }

    writeOut("");
    writeOut("Default subgroup size: ", device->getPhysicalDeviceSubgroupProperties().subgroupSize);
    if (device->getPhysicalDeviceVulkan13Features().subgroupSizeControl
            && (device->getPhysicalDeviceVulkan13Properties().requiredSubgroupSizeStages & VK_SHADER_STAGE_COMPUTE_BIT) != 0) {
        writeOut("Min subgroup size: ", device->getPhysicalDeviceVulkan13Properties().minSubgroupSize);
        writeOut("Max subgroup size: ", device->getPhysicalDeviceVulkan13Properties().maxSubgroupSize);
    }

    writeOut("");
    writeOut("Max memory allocations: ", device->getLimits().maxMemoryAllocationCount);
    writeOut(
            "Max storage buffer range: ",
            sgl::getNiceMemoryStringDifference(device->getLimits().maxStorageBufferRange, 2, true));
    if (device->getPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1) {
        writeOut(
                "Max memory allocation size: ",
                sgl::getNiceMemoryStringDifference(device->getMaxMemoryAllocationSize(), 2, true));
    }
    writeOut("Supports shader 64-bit indexing: ", device->getShader64BitIndexingFeaturesEXT().shader64BitIndexing ? "Yes" : "No");
    writeOut("alignof(std::max_align_t): ", alignof(std::max_align_t));
    writeOut("Min imported host pointer alignment: ", device->getMinImportedHostPointerAlignment());

    std::vector<std::string> flagsStringMap = {
            "device local",  // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            "host visible",  // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            "host coherent", // VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            "host cached"    // VK_MEMORY_PROPERTY_HOST_CACHED_BIT
    };
    const VkPhysicalDeviceMemoryProperties& deviceMemoryProperties = device->getMemoryProperties();
    for (uint32_t heapIdx = 0; heapIdx < deviceMemoryProperties.memoryHeapCount; heapIdx++) {
        VkMemoryPropertyFlagBits typeFlags{};
        for (uint32_t memoryTypeIdx = 0; memoryTypeIdx < deviceMemoryProperties.memoryTypeCount; memoryTypeIdx++) {
            if (deviceMemoryProperties.memoryTypes[memoryTypeIdx].heapIndex == heapIdx) {
                typeFlags = VkMemoryPropertyFlagBits(typeFlags | deviceMemoryProperties.memoryTypes[memoryTypeIdx].propertyFlags);
            }
        }
        std::string memoryHeapInfo;
        if (typeFlags != 0) {
            memoryHeapInfo = " (";
            typeFlags = VkMemoryPropertyFlagBits(typeFlags & 0xF);
            auto numEntries = int(sgl::popcount(uint32_t(typeFlags)));
            int entryIdx = 0;
            for (int i = 0; i < 4; i++) {
                auto flag = VkMemoryPropertyFlagBits(1 << i);
                if ((typeFlags & flag) != 0) {
                    memoryHeapInfo += flagsStringMap[i];
                    if (entryIdx != numEntries - 1) {
                        memoryHeapInfo += ", ";
                    }
                    entryIdx++;
                }
            }
            memoryHeapInfo += ")";
        }
        bool hasTypeDeviceLocal = (typeFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
        bool isHeapDeviceLocal = (deviceMemoryProperties.memoryHeaps[heapIdx].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0;
        if (hasTypeDeviceLocal != isHeapDeviceLocal) {
            sgl::Logfile::get()->writeError("Encountered memory heap with mismatching heap and type flags.");
        }
        writeOut(
                "Memory heap #", heapIdx, ": ",
                sgl::getNiceMemoryStringDifference(deviceMemoryProperties.memoryHeaps[heapIdx].size, 2, true),
                memoryHeapInfo);
    }

    writeOut("");
    writeOut("Shader int8 support: ", bool(device->getPhysicalDeviceVulkan12Features().shaderInt8));
    writeOut("Shader float16 support: ", bool(device->getPhysicalDeviceVulkan12Features().shaderFloat16));
    writeOut("Shader bfloat16 support: ", bool(device->getPhysicalDeviceShaderBfloat16Features().shaderBFloat16Type));

    checkCooperativeMatrixFeaturesKHR(device);
    checkCooperativeMatrixFeaturesNV2(device);
    checkCooperativeVectorFeaturesNV(device);
}

#ifdef __linux__
void querySingleImageDrmFormatModifiers(sgl::vk::Device* device, VkFormat format, std::ofstream& formatFile) {
    VkDrmFormatModifierPropertiesListEXT drmFormatModifierPropertiesList{};
    drmFormatModifierPropertiesList.sType = VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_EXT;
    //VkDrmFormatModifierPropertiesList2EXT drmFormatModifierPropertiesList2{};
    //drmFormatModifierPropertiesList2.sType = VK_STRUCTURE_TYPE_DRM_FORMAT_MODIFIER_PROPERTIES_LIST_2_EXT;
    VkFormatProperties2 formatProperties2{};
    formatProperties2.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
    formatProperties2.pNext = &drmFormatModifierPropertiesList;
    device->getPhysicalDeviceFormatProperties2(format, formatProperties2);
    formatFile << "<br>\n";
    formatFile << "Format name: <b>" << convertVkFormatToString(format) << "</b><br>\n";
    formatFile << "Linear tiling features: " << convertVkFormatFeatureFlagsToString(
        formatProperties2.formatProperties.linearTilingFeatures) << "<br>\n";
    formatFile << "Optimal tiling features: " << convertVkFormatFeatureFlagsToString(
        formatProperties2.formatProperties.optimalTilingFeatures) << "<br>\n";
    formatFile << "Buffer features: " << convertVkFormatFeatureFlagsToString(
        formatProperties2.formatProperties.bufferFeatures) << "<br>\n";
    if (drmFormatModifierPropertiesList.drmFormatModifierCount != 0) {
        drmFormatModifierPropertiesList.pDrmFormatModifierProperties =
            new VkDrmFormatModifierPropertiesEXT[drmFormatModifierPropertiesList.drmFormatModifierCount];
        device->getPhysicalDeviceFormatProperties2(format, formatProperties2);
        formatFile << "<br>\n";
        formatFile << "<table><tr><th>Modifier</th><th>Plane Count</th><th>Tiling Features</th></tr>\n";
        for (uint32_t i = 0; i < drmFormatModifierPropertiesList.drmFormatModifierCount; i++) {
            auto& drmFormatModifierProp = drmFormatModifierPropertiesList.pDrmFormatModifierProperties[i];
            formatFile << "<tr>";
            formatFile << "<td>" << convertDrmFormatModifierToString(drmFormatModifierProp.drmFormatModifier) << "</td>";
            formatFile << "<td>" << drmFormatModifierProp.drmFormatModifierPlaneCount << "</td>";
            formatFile << "<td>" << convertVkFormatFeatureFlagsToString(drmFormatModifierProp.drmFormatModifierTilingFeatures) << "</td>";
            formatFile << "</tr>\n";
        }
        formatFile << "</table>\n";
        delete[] drmFormatModifierPropertiesList.pDrmFormatModifierProperties;
    }
    formatFile << "<br><hr>\n";
}

void queryImageDrmFormatModifiers(size_t deviceIdx, sgl::vk::Device* device) {
    std::ofstream formatFile("FormatInfoDRM_" + std::to_string(deviceIdx) + ".html");
    formatFile << "<html><head><title>Vulkan Image Format DRM Info</title>";
    formatFile << "\n<style>\n";
    formatFile << "table {\ntext-align: center;\n}\n";
    formatFile << "</style>\n";
    formatFile << "</head>";
    formatFile << "<body><font face='courier new'>";
    formatFile << "<table width='100%%' bgcolor='#E0E0E5'><tr><td><font face='arial' size='+2'>";
    formatFile << "Vulkan Image Format DRM Info";
    formatFile << "</font></td></tr></table>\n<br>";

    formatFile << "Device name: " << device->getDeviceName() << "<br>\n";
    if (device->getPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1) {
        formatFile << "Device driver name: " << device->getDeviceDriverName() << "<br>\n";
        formatFile << "Device driver info: " << device->getDeviceDriverInfo() << "<br>\n";
        formatFile << "Device driver ID: " << device->getDeviceDriverId() << "<br>\n";
    }
    formatFile << "<br><hr>\n";

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R8G8B8A8_UNORM, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_B8G8R8A8_UNORM, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R8G8B8A8_SRGB, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_B8G8R8A8_SRGB, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16_UNORM, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16G16_UNORM, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16G16B16A16_UNORM, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_D32_SFLOAT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32_SFLOAT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32_SFLOAT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32B32A32_SFLOAT, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16_SFLOAT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16G16_SFLOAT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R16G16B16A16_SFLOAT, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32_UINT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32_UINT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32B32A32_UINT, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32_SINT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32_SINT, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R32G32B32A32_SINT, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_B10G11R11_UFLOAT_PACK32, formatFile);
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_R64_UINT, formatFile);

    querySingleImageDrmFormatModifiers(device, VK_FORMAT_G8_B8R8_2PLANE_420_UNORM, formatFile); // NV12
    querySingleImageDrmFormatModifiers(device, VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16, formatFile); // P010

    formatFile << "</font></body></html>";
    formatFile.close();
}
#endif

int main(int argc, char *argv[]) {
#ifdef __linux__
    bool shallTestDrmFormatModifiers = false;
#endif
#ifdef _WIN32
    bool shallTestWglExperimental = false;
#endif
    for (int i = 1; i < argc; i++) {
        std::string command = argv[i];
        if (command == "--help" || command == "-h") {
            std::cout << "QueryVkCoopMat: Queries Vulkan cooperative matrix support." << std::endl;
#ifdef __linux__
            std::cout << "Optional argument: --test-drm-format (queries Linux DRM image format modifiers)" << std::endl;
#endif
#ifdef _WIN32
            std::cout << "Optional argument: --wgl (queries WGL contexts for each device; experimental)" << std::endl;
#endif
        }
#ifdef __linux__
        else if (command == "--test-drm-format" || command == "--test-drm-formats" || command == "--drm-formats"
                || "--drm") {
            shallTestDrmFormatModifiers = true;
        }
#endif
#ifdef _WIN32
        else if (command == "--wgl") {
            shallTestWglExperimental = true;
        }
#endif
        else {
            throw std::runtime_error("Invalid command line arguments.");
        }
    }

    sgl::Logfile::get()->createLogfile("Logfile.html", "QueryVkCoopMat");
    sgl::Logfile::get()->write("\n<style>\n");
    sgl::Logfile::get()->write("table {\nborder-spacing: 10px 0;\n}\n");
    sgl::Logfile::get()->write("</style>\n");

    auto* instance = new sgl::vk::Instance;
    instance->createInstance({}, false);
#ifdef __linux__
    sgl::Logfile::get()->write("<br>\n");
    bool isEglInitialized = loadEglLibrary();
#endif
#ifdef _WIN32
    bool isWglInitialized = false;
    if (shallTestWglExperimental) {
        isWglInitialized = initWglPatch();
    }
#endif

    std::vector<const char*> optionalDeviceExtensions;
    std::vector<const char*> requiredDeviceExtensions = {
            VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
    };
    sgl::vk::DeviceFeatures requestedDeviceFeatures{};
    requestedDeviceFeatures.optionalVulkan12Features.shaderInt8 = VK_TRUE;
    requestedDeviceFeatures.optionalVulkan12Features.shaderFloat16 = VK_TRUE;
    requestedDeviceFeatures.optionalVulkan11Features.storageBuffer16BitAccess = VK_TRUE;
    requestedDeviceFeatures.optionalVulkan12Features.vulkanMemoryModel = VK_TRUE; // For cooperative matrices.
    requestedDeviceFeatures.optionalVulkan12Features.vulkanMemoryModelDeviceScope = VK_TRUE; // For cooperative matrices.
    requestedDeviceFeatures.optionalVulkan13Features.subgroupSizeControl = VK_TRUE;
    optionalDeviceExtensions.push_back(VK_KHR_SHADER_BFLOAT16_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_VECTOR_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_EXT_SHADER_64BIT_INDEXING_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME);
    optionalDeviceExtensions.push_back(VK_EXT_EXTERNAL_MEMORY_HOST_EXTENSION_NAME);
#ifdef __linux__
    if (shallTestDrmFormatModifiers) {
        optionalDeviceExtensions.push_back(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME);
    }
#endif

    std::vector<VkPhysicalDevice> physicalDevices = sgl::vk::enumeratePhysicalDevices(instance);
    std::vector<VkPhysicalDevice> suitablePhysicalDevices;
    for (auto& physicalDevice : physicalDevices) {
        if (sgl::vk::checkIsPhysicalDeviceSuitable(
                instance, physicalDevice, nullptr, requiredDeviceExtensions, requestedDeviceFeatures, true)) {
            suitablePhysicalDevices.push_back(physicalDevice);
        }
    }
    for (size_t i = 0; i < suitablePhysicalDevices.size(); i++) {
        if (i != 0) {
            std::cout << std::endl << "--------------------------------------------" << std::endl << std::endl;
        }
        sgl::Logfile::get()->write("<br><hr><br>\n");
        auto physicalDevice = suitablePhysicalDevices.at(i);
        auto* device = new sgl::vk::Device;
        device->createDeviceHeadlessFromPhysicalDevice(
                instance, physicalDevice, requiredDeviceExtensions,
                optionalDeviceExtensions, requestedDeviceFeatures, true);
#ifdef __linux__
        if (isEglInitialized) {
            checkEglFeatures(device);
        }
#endif
#ifdef _WIN32
        if (isWglInitialized) {
            checkWglFeatures(device);
        }
#endif
        checkCooperativeMatrixFeatures(device);
#ifdef __linux__
        if (shallTestDrmFormatModifiers && device->getApiVersion() >= VK_API_VERSION_1_3
                && device->isDeviceExtensionSupported(VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME)) {
            queryImageDrmFormatModifiers(i, device);
        }
#endif
        delete device;
        if (i == suitablePhysicalDevices.size() - 1) {
            sgl::Logfile::get()->write("<br><hr>\n");
        }
    }

#ifdef __linux__
    if (isEglInitialized) {
        releaseEglLibrary();
    }
#endif
#ifdef _WIN32
    if (isWglInitialized) {
        freeWglPatch();
    }
#endif
    delete instance;

#ifdef _WIN32
    pauseIfAppOwnsConsole();
#endif

    return 0;
}
