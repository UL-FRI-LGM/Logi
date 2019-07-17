#version 450

layout (location = 0) in vec3 inColor;
layout (location = 0) out vec4 outFragColor;

layout(push_constant) uniform PushConstants {
    layout(offset = 8) float val_a;
    layout(offset = 16) vec2 val_b;
} pConst;


void main() {
  outFragColor = vec4(inColor + vec3(pConst.val_a, pConst.val_b), 1.0);
}