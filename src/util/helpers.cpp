#include "util/helpers.h"

namespace logi {

bool hasRequestedFeatures(const vk::PhysicalDeviceFeatures& super, const vk::PhysicalDeviceFeatures& sub) {
	return (!(sub.robustBufferAccess) || super.robustBufferAccess) &&
		(!(sub.fullDrawIndexUint32) || super.fullDrawIndexUint32) &&
		(!(sub.imageCubeArray) || super.imageCubeArray) &&
		(!(sub.independentBlend) || super.independentBlend) &&
		(!(sub.geometryShader) || super.geometryShader) &&
		(!(sub.tessellationShader) || super.tessellationShader) &&
		(!(sub.sampleRateShading) || super.sampleRateShading) &&
		(!(sub.dualSrcBlend) || super.dualSrcBlend) &&
		(!(sub.logicOp) || super.logicOp) &&
		(!(sub.multiDrawIndirect) || super.multiDrawIndirect) &&
		(!(sub.drawIndirectFirstInstance) || super.drawIndirectFirstInstance) &&
		(!(sub.depthClamp) || super.depthClamp) &&
		(!(sub.depthBiasClamp) || super.depthBiasClamp) &&
		(!(sub.fillModeNonSolid) || super.fillModeNonSolid) &&
		(!(sub.depthBounds) || super.depthBounds) &&
		(!(sub.wideLines) || super.wideLines) &&
		(!(sub.largePoints) || super.largePoints) &&
		(!(sub.alphaToOne) || super.alphaToOne) &&
		(!(sub.multiViewport) || super.multiViewport) &&
		(!(sub.samplerAnisotropy) || super.samplerAnisotropy) &&
		(!(sub.textureCompressionETC2) || super.textureCompressionETC2) &&
		(!(sub.textureCompressionASTC_LDR) || super.textureCompressionASTC_LDR) &&
		(!(sub.textureCompressionBC) || super.textureCompressionBC) &&
		(!(sub.occlusionQueryPrecise) || super.occlusionQueryPrecise) &&
		(!(sub.pipelineStatisticsQuery) || super.pipelineStatisticsQuery) &&
		(!(sub.vertexPipelineStoresAndAtomics) || super.vertexPipelineStoresAndAtomics) &&
		(!(sub.fragmentStoresAndAtomics) || super.fragmentStoresAndAtomics) &&
		(!(sub.shaderTessellationAndGeometryPointSize) || super.shaderTessellationAndGeometryPointSize) &&
		(!(sub.shaderImageGatherExtended) || super.shaderImageGatherExtended) &&
		(!(sub.shaderStorageImageExtendedFormats) || super.shaderStorageImageExtendedFormats) &&
		(!(sub.shaderStorageImageMultisample) || super.shaderStorageImageMultisample) &&
		(!(sub.shaderStorageImageReadWithoutFormat) || super.shaderStorageImageReadWithoutFormat) &&
		(!(sub.shaderStorageImageWriteWithoutFormat) || super.shaderStorageImageWriteWithoutFormat) &&
		(!(sub.shaderUniformBufferArrayDynamicIndexing) || super.shaderUniformBufferArrayDynamicIndexing) &&
		(!(sub.shaderSampledImageArrayDynamicIndexing) || super.shaderSampledImageArrayDynamicIndexing) &&
		(!(sub.shaderStorageBufferArrayDynamicIndexing) || super.shaderStorageBufferArrayDynamicIndexing) &&
		(!(sub.shaderStorageImageArrayDynamicIndexing) || super.shaderStorageImageArrayDynamicIndexing) &&
		(!(sub.shaderClipDistance) || super.shaderClipDistance) &&
		(!(sub.shaderCullDistance) || super.shaderCullDistance) &&
		(!(sub.shaderFloat64) || super.shaderFloat64) &&
		(!(sub.shaderInt64) || super.shaderInt64) &&
		(!(sub.shaderInt16) || super.shaderInt16) &&
		(!(sub.shaderResourceResidency) || super.shaderResourceResidency) &&
		(!(sub.shaderResourceMinLod) || super.shaderResourceMinLod) &&
		(!(sub.sparseBinding) || super.sparseBinding) &&
		(!(sub.sparseResidencyBuffer) || super.sparseResidencyBuffer) &&
		(!(sub.sparseResidencyImage2D) || super.sparseResidencyImage2D) &&
		(!(sub.sparseResidencyImage3D) || super.sparseResidencyImage3D) &&
		(!(sub.sparseResidency2Samples) || super.sparseResidency2Samples) &&
		(!(sub.sparseResidency4Samples) || super.sparseResidency4Samples) &&
		(!(sub.sparseResidency8Samples) || super.sparseResidency8Samples) &&
		(!(sub.sparseResidency16Samples) || super.sparseResidency16Samples) &&
		(!(sub.variableMultisampleRate) || super.variableMultisampleRate) &&
		(!(sub.inheritedQueries) || super.inheritedQueries);
}

}
