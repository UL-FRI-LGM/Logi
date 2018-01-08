#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D sampA[2];
layout(binding = 1) uniform sampler3D sampB;


layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

layout (input_attachment_index=0, set=1, binding=0) uniform subpassInput myInputAttachment;

void main() {
    outColor = vec4(fragColor, 1.0) * texture(sampA[1], fragColor.xy) * texture(sampA[0], fragColor.xy) * texture(sampB, fragColor);
}