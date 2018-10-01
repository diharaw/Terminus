#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 texcoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform texture2D u_Texture;
layout(set = 1, binding = 0) uniform sampler u_Sampler;

void main() 
{
    outColor = vec4(texture(sampler2D(u_Texture, u_Sampler), texcoord), 1.0);
}