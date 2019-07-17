#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 model;
	vec4 viewPos;
} ubo_;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outViewVec;
layout (location = 3) out vec3 outLightVec;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
	outUV = inUV;
	vec3 worldPos = vec3(ubo_.model * vec4(inPos, 1.0));

	gl_Position = ubo_.projection * ubo_.model * vec4(inPos.xyz, 1.0);

    vec4 pos = ubo_.model * vec4(inPos, 1.0);
	outNormal = mat3(inverse(transpose(ubo_.model))) * inNormal;
	vec3 lightPos = vec3(0.0);
	vec3 lPos = mat3(ubo_.model) * lightPos.xyz;
    outLightVec = lPos - pos.xyz;
    outViewVec = ubo_.viewPos.xyz - pos.xyz;
}
