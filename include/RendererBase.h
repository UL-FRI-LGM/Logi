/*
* Vulkan Renderer base class.
*
* Copyright (C) 2017 by Primoz Lavric
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#ifndef RENDERER_BASE_H_
#define RENDERER_BASE_H_

#include <vulkan\vulkan.hpp>

class RendererBase
{
public:
	RendererBase();
	~RendererBase();

private:
	vk::Instance vk_instance_;


};

#endif // !RENDERER_BASE_H_