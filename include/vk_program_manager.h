/*
* Project       : VulkanRenderer
*
* Author        : Lana Besevic
*
* For the terms and conditions of redistribution or use of this software
* refer to the file LICENSE located in the root level directory
* of the project.
*/

#ifndef _VKR_VK_PROGRAM_MANAGER_H_
#define _VKR_VK_PROGRAM_MANAGER_H_

#include <string>
#include <map>

class VulkanProgramManager {
public:
	enum class ShaderType {
		VERTEX = 0,
		GEOMETRY = 1,
		FRAGMENT = 2,
		COMPUTE = 3
	};

	VulkanProgramManager(const std::string& specification_path);

	virtual ~VulkanProgramManager() = default;
private:
	struct example {
		std::string path;
		ShaderType ShaderType;
	};

	std::map<std::string, std::string> mp_;

};

#endif  // _VKR_VK_PROGRAM_MANAGER_H_