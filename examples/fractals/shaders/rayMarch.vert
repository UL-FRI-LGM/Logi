#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

// layout (set = 0, binding = 0) uniform UBO {
// 	mat4 projectionMatrix;
// 	mat4 modelMatrix;
// 	mat4 viewMatrix;
// } ubo_;

layout (location = 0) out vec2 outUV;

out gl_PerVertex {
    vec4 gl_Position;
};


void main() {
	outUV = inUV;
	gl_Position = vec4(inPos.xyz, 1.0);
	// gl_Position = ubo_.projectionMatrix * ubo_.viewMatrix * ubo_.modelMatrix * vec4(inPos.xyz, 1.0);
}