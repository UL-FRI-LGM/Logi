#version 450

layout(binding = 0) uniform UBO {
	mat4 modelView;
	mat4 projection;
} ubo_;

layout(location = 0) in vec3 inPositions;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCord;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	fragColor = inColor;
	fragTexCord = inTexCord;

	gl_Position = ubo_.projection * ubo_.modelView * vec4(inPositions, 1.0);
	// gl_Position = vec4(inPositions, 1.0);
}