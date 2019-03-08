#include "logi/util/FormatConversion.h"
namespace logi {

vk::Format getVertexBufferFormat(const spirv_cross::SPIRType& format_info) {
	
	switch (format_info.width) {
	case 8:
		// Start: Determine type.
		switch (format_info.basetype) {
		case spirv_cross::SPIRType::BaseType::Int:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR8Sint;
			case 2:
				return vk::Format::eR8G8Sint;
			case 3:
				return vk::Format::eR8G8B8Sint;
			case 4:
				return vk::Format::eR8G8B8A8Sint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::UInt:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR8Uint;
			case 2:
				return vk::Format::eR8G8Uint;
			case 3:
				return vk::Format::eR8G8B8Uint;
			case 4:
				return vk::Format::eR8G8B8A8Uint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		default:
			throw std::runtime_error("Failed to determine VkFormat.");
			break;
		}
		// End: Determine type.
		break;
	case 16:
		// Start: Determine type.
		switch (format_info.basetype) {
		case spirv_cross::SPIRType::BaseType::Int:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR16Sint;
			case 2:
				return vk::Format::eR16G16Sint;
			case 3:
				return vk::Format::eR16G16B16Sint;
			case 4:
				return vk::Format::eR16G16B16A16Sint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::UInt:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR16Uint;
			case 2:
				return vk::Format::eR16G16Uint;
			case 3:
				return vk::Format::eR16G16B16Uint;
			case 4:
				return vk::Format::eR16G16B16A16Uint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::Float:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR16Sfloat;
			case 2:
				return vk::Format::eR16G16Sfloat;
			case 3:
				return vk::Format::eR16G16B16Sfloat;
			case 4:
				return vk::Format::eR16G16B16A16Sfloat;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		default:
			throw std::runtime_error("Failed to determine VkFormat.");
			break;
		}
		// End: Determine type.
		break;

	case 32:
		// Start: Determine type.
		switch (format_info.basetype) {
		case spirv_cross::SPIRType::BaseType::Int:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR32Sint;
			case 2:
				return vk::Format::eR32G32Sint;
			case 3:
				return vk::Format::eR32G32B32Sint;
			case 4:
				return vk::Format::eR32G32B32A32Sint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::UInt:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR32Uint;
			case 2:
				return vk::Format::eR32G32Uint;
			case 3:
				return vk::Format::eR32G32B32Uint;
			case 4:
				return vk::Format::eR32G32B32A32Uint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::Float:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR32Sfloat;
			case 2:
				return vk::Format::eR32G32Sfloat;
			case 3:
				return vk::Format::eR32G32B32Sfloat;
			case 4:
				return vk::Format::eR32G32B32A32Sfloat;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		default:
			throw std::runtime_error("Failed to determine VkFormat.");
			break;
		}
		// End: Determine type.
		break;

	case 64:
		// Start: Determine type.
		switch (format_info.basetype) {
		case spirv_cross::SPIRType::BaseType::Int:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR64Sint;
			case 2:
				return vk::Format::eR64G64Sint;
			case 3:
				return vk::Format::eR64G64B64Sint;
			case 4:
				return vk::Format::eR64G64B64A64Sint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::UInt:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR64Uint;
			case 2:
				return vk::Format::eR64G64Uint;
			case 3:
				return vk::Format::eR64G64B64Uint;
			case 4:
				return vk::Format::eR64G64B64A64Uint;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		case spirv_cross::SPIRType::BaseType::Double:
			// Start: Determine vector size.
			switch (format_info.vecsize) {
			case 1:
				return vk::Format::eR64Sfloat;
			case 2:
				return vk::Format::eR64G64Sfloat;
			case 3:
				return vk::Format::eR64G64B64Sfloat;
			case 4:
				return vk::Format::eR64G64B64A64Sfloat;
			default:
				throw std::runtime_error("Failed to determine VkFormat.");
			}
			// End: Determine vector size.
			break;

		default:
			throw std::runtime_error("Failed to determine VkFormat.");
			break;
		}
		// End: Determine type.
		break;
	
	default:
		throw std::runtime_error("Failed to determine VkFormat.");
	}
}

}