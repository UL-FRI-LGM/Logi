#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in dvec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in mat4 test;

layout(binding = 0) uniform UniformBufferObject {
	vec3 test;
	ivec2 a;
} ubo[2];

layout(push_constant) uniform PushConsts {
	vec3 lightPos;
	int abc;
} pushConsts;

out gl_PerVertex {
    vec4 gl_Position;
};

layout(location = 0) out vec3 fragColor;


vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
	if (ubo[1].a.x > 0) {
    gl_Position = vec4(position + ubo[0].test, 0.0) * test;
	}
	//vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex] + color * pushConsts.lightPos * pushConsts.abc;
}