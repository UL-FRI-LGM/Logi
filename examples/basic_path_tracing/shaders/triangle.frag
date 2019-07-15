#version 450

#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0) uniform sampler2D samplerColor;

layout(std140, binding = 1) uniform UniformBufferObject {
	float frame_number;
} ubo_;

layout (location = 0) in vec2 inUV;

layout (location = 0) out vec4 outFragColor;

void main() 
{
  outFragColor = texture(samplerColor, vec2(inUV.s, 1.0 - inUV.t)) / ubo_.frame_number;
}