#version 440 core

uniform sampler2D u_Diffuse; //#slot 0

in vec2 PS_IN_Texcoord;
out vec4 PS_OUT_Color;

void main()
{
	PS_OUT_Color = vec4(texture(u_Diffuse, PS_IN_Texcoord).rgb, 1.0f);
}