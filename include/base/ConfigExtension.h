/*
*
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef BASE_CONFIG_EXTENSION_H
#define BASE_CONFIG_EXTENSION_H

namespace logi {


/**
 * @brief	Interface for config extensions.
 */
class ConfigExtension {
public:
	/**
	 * @brief	Buildes extension configuration and return void pointer to it.	
	 *
	 * @return	Void pointer to the configuration structure.
	 */
	virtual void* buildConfig() const = 0;

	virtual ~ConfigExtension() = default;
};


static void addNextExtensions(const void** next) {
	*next = nullptr;
}


template <typename Ext, typename... Extensions>
static void addNextExtensions(const void** next, Ext current, Extensions... remaining) {
	auto* config = current.buildConfig();
	*next = config;
	addNextExtensions(&config->pNext, remaining...);
}

class TestExtensionA {
public:
	vk::RenderPassBeginInfo* buildConfig() const {
		return begin_info_.get();
	}

private:
	std::shared_ptr<vk::RenderPassBeginInfo> begin_info_{ std::make_shared<vk::RenderPassBeginInfo>() };
};

class TestExtensionB {
public:
	vk::GraphicsPipelineCreateInfo* buildConfig() const {
		return begin_info_.get();
	}

private:
	std::shared_ptr<vk::GraphicsPipelineCreateInfo> begin_info_{ std::make_shared<vk::GraphicsPipelineCreateInfo>() };
};

}

#endif