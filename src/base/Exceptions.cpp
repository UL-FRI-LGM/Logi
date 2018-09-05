#include "base/Exceptions.h"

namespace logi {

LogiException::LogiException(const std::string& message)
    : std::runtime_error(message) {}

LogiException::LogiException(const char* const message)
    : std::runtime_error(message) {}

VulkanError::VulkanError(const vk::Result result, const std::string& message)
    : LogiException(message), result_(result) {}

VulkanError::VulkanError(const vk::Result result, const char* const message)
    : LogiException(message), result_(result) {}

vk::Result VulkanError::result() const {
    return result_;
}

IllegalInvocation::IllegalInvocation(const std::string& message)
    : LogiException(message) {}

IllegalInvocation::IllegalInvocation(const char* const message)
    : LogiException(message) {}

}
