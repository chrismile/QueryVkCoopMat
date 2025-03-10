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
#include <map>

#include <Utils/File/Logfile.hpp>
#include <Graphics/Vulkan/Utils/Instance.hpp>
#include <Graphics/Vulkan/Utils/Device.hpp>

/*const char* const COMPONENT_TYPE_NAMES[] = {
        "float16", // VK_COMPONENT_TYPE_FLOAT16_KHR,
        "float32", // VK_COMPONENT_TYPE_FLOAT32_KHR,
        "float64", // VK_COMPONENT_TYPE_FLOAT64_KHR,
        "sint8",   // VK_COMPONENT_TYPE_SINT8_KHR,
        "sint16",  // VK_COMPONENT_TYPE_SINT16_KHR,
        "sint32",  // VK_COMPONENT_TYPE_SINT32_KHR,
        "sint64",  // VK_COMPONENT_TYPE_SINT64_KHR,
        "uint8",   // VK_COMPONENT_TYPE_UINT8_KHR,
        "uint16",  // VK_COMPONENT_TYPE_UINT16_KHR,
        "uint32",  // VK_COMPONENT_TYPE_UINT32_KHR,
        "uint64",  // VK_COMPONENT_TYPE_UINT64_KHR,
};*/

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

template<typename... T>
void writeOut(T... args) {
    std::string text = (std::string() + ... + sgl::toString(args));
    sgl::Logfile::get()->write(text, sgl::BLACK);
    std::cout << text << std::endl;
}

void checkCooperativeMatrixFeaturesKHR(sgl::vk::Device* device) {
    if (!device->getCooperativeMatrixFeaturesKHR().cooperativeMatrix) {
        writeOut("");
        writeOut("VK_KHR_cooperative_matrix is not supported.");
        return;
    }

#ifdef VK_KHR_cooperative_matrix
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
                << "\nsaturatingAccumulation: " << props.saturatingAccumulation
                << "\nscope: " << getScopeString(props.scope)
                << "\n" << std::endl;
        sgl::Logfile::get()->write("<tr></tr>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.MSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.NSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.KSize) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.AType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.BType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.CType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.ResultType) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.saturatingAccumulation) +"</td>");
        sgl::Logfile::get()->write("<td>" + getScopeString(props.scope) +"</td>");
        sgl::Logfile::get()->write("</tr>\n");
    }
    sgl::Logfile::get()->write("</table>\n");
#endif
}

void checkCooperativeMatrixFeaturesNV2(sgl::vk::Device* device) {
#ifdef VK_NV_cooperative_matrix2
    if (!device->isDeviceExtensionSupported(VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME)) {
#endif
        writeOut("");
        writeOut("VK_NV_cooperative_matrix2 is not supported.");
        return;
#ifdef VK_NV_cooperative_matrix2
    }
#endif

#ifdef VK_NV_cooperative_matrix2
    const auto& features = device->getCooperativeMatrix2FeaturesNV();
    const auto& properties = device->getCooperativeMatrix2PropertiesNV();
    const auto& flexibleDimensionsProperties = device->getSupportedCooperativeMatrixFlexibleDimensionsPropertiesNV();
    writeOut("");
    writeOut("VK_NV_cooperative_matrix2 properties:");
    writeOut("cooperativeMatrixWorkgroupScope: ", features.cooperativeMatrixWorkgroupScope);
    writeOut("cooperativeMatrixFlexibleDimensions: ", features.cooperativeMatrixFlexibleDimensions);
    writeOut("cooperativeMatrixReductions: ", features.cooperativeMatrixReductions);
    writeOut("cooperativeMatrixConversions: ", features.cooperativeMatrixConversions);
    writeOut("cooperativeMatrixPerElementOperations: ", features.cooperativeMatrixPerElementOperations);
    writeOut("cooperativeMatrixTensorAddressing: ", features.cooperativeMatrixTensorAddressing);
    writeOut("cooperativeMatrixBlockLoads: ", features.cooperativeMatrixBlockLoads);
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
                << "\nsaturatingAccumulation: " << props.saturatingAccumulation
                << "\nscope: " << getScopeString(props.scope)
                << "\nworkgroupInvocations: " << props.workgroupInvocations
                << "\n" << std::endl;
        sgl::Logfile::get()->write("<tr></tr>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.MGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.NGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.KGranularity) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.AType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.BType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.CType) +"</td>");
        sgl::Logfile::get()->write("<td>" + getComponentTypeString(props.ResultType) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.saturatingAccumulation) +"</td>");
        sgl::Logfile::get()->write("<td>" + getScopeString(props.scope) +"</td>");
        sgl::Logfile::get()->write("<td>" + std::to_string(props.workgroupInvocations) +"</td>");
        sgl::Logfile::get()->write("</tr>\n");
    }
    sgl::Logfile::get()->write("</table>\n");
#endif
}

void checkCooperativeMatrixFeatures(sgl::vk::Device* device) {
    sgl::Logfile::get()->write("<br>");
    writeOut(std::string() + "Device name: " + device->getDeviceName());
    if (device->getPhysicalDeviceProperties().apiVersion >= VK_API_VERSION_1_1) {
        writeOut("Device driver name: ", device->getDeviceDriverName());
        writeOut("Device driver info: ", device->getDeviceDriverInfo());
        writeOut("Device driver ID: ", device->getDeviceDriverId());
    }

    writeOut("");
    writeOut("Default subgroup size: ", device->getPhysicalDeviceSubgroupProperties().subgroupSize);
    if (device->getPhysicalDeviceVulkan13Features().subgroupSizeControl
            && (device->getPhysicalDeviceVulkan13Properties().requiredSubgroupSizeStages & VK_SHADER_STAGE_COMPUTE_BIT) != 0) {
        writeOut("Min subgroup size: ", device->getPhysicalDeviceVulkan13Properties().minSubgroupSize);
        writeOut("Max subgroup size: ", device->getPhysicalDeviceVulkan13Properties().maxSubgroupSize);
    }

    checkCooperativeMatrixFeaturesKHR(device);
    checkCooperativeMatrixFeaturesNV2(device);
}

int main() {
    sgl::Logfile::get()->createLogfile("Logfile.html", "QueryVkCoopMat");

    auto* instance = new sgl::vk::Instance;
    instance->createInstance({}, false);
    auto* device = new sgl::vk::Device;
    std::vector<const char*> optionalDeviceExtensions;
    sgl::vk::DeviceFeatures requestedDeviceFeatures{};
    requestedDeviceFeatures.optionalVulkan12Features.shaderFloat16 = VK_TRUE;
    requestedDeviceFeatures.optionalVulkan11Features.storageBuffer16BitAccess = VK_TRUE;
    requestedDeviceFeatures.optionalVulkan12Features.vulkanMemoryModel = VK_TRUE; // For cooperative matrices.
    requestedDeviceFeatures.optionalVulkan12Features.vulkanMemoryModelDeviceScope = VK_TRUE; // For cooperative matrices.
#ifdef VK_VERSION_1_3
    requestedDeviceFeatures.optionalVulkan13Features.subgroupSizeControl = VK_TRUE;
#endif
#ifdef VK_NV_cooperative_matrix
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME);
#endif
#ifdef VK_NV_cooperative_matrix2
    optionalDeviceExtensions.push_back(VK_NV_COOPERATIVE_MATRIX_2_EXTENSION_NAME);
#endif
#ifdef VK_KHR_cooperative_matrix
    optionalDeviceExtensions.push_back(VK_KHR_COOPERATIVE_MATRIX_EXTENSION_NAME);
#endif
    device->createDeviceHeadless(
            instance, {
                    VK_EXT_SCALAR_BLOCK_LAYOUT_EXTENSION_NAME, VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
            },
            optionalDeviceExtensions, requestedDeviceFeatures);

    checkCooperativeMatrixFeatures(device);

    delete device;
    delete instance;

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}
