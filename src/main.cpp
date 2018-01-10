#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "examples/ExampleA.h"


int main() {
	vkr::ExampleA::runExample();
	system("pause");
}