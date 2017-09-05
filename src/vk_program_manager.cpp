#include "vk_program_manager.h"

#include <string>
#include <fstream>
#include <json.hpp>
#include <iostream>

VulkanProgramManager::VulkanProgramManager(const std::string& specification_path) {
	std::ifstream spec_file(specification_path);
	std::string file_data((std::istreambuf_iterator<char>(spec_file)), std::istreambuf_iterator<char>());

	std::cout << file_data << std::endl;
}
