/*
* Shader manager.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <vector>
#include <map>
#include <vulkan/vulkan.hpp>
#include <filesystem>

namespace vkr {
namespace filesystem = std::experimental::filesystem;  ///< todo(plavric): Change when the filesystem will no longer be experimental.

class ShaderManager {
public:
	ShaderManager(const std::string& shaders_path);

protected:
	void loadShaders();

	vk::ShaderModule getShaderModule(const std::string& name);
private:
	struct ShaderModuleInfo {
		std::vector<std::pair<vk::ShaderStageFlagBits, filesystem::path>> shaders;
		std::map<vk::PhysicalDevice, vk::ShaderModule> compiled_modules;
	};

	static std::vector<char> readShaderFile(const filesystem::path& filename);

	filesystem::path shaders_path_;
	std::map<std::string, ShaderModuleInfo> shader_modules_;
};

}

#endif