#include <iostream> ///< For std::cout << "To zelim sprintati na izhod" << std::endl;
#include <fstream> ///< File reading.. ifstream(path).
#include "vk_program_manager.h"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

int main() {
	VulkanProgramManager program_manager("./resources/shaders/shader_specification.json");

	system("pause");
	return 0;
}