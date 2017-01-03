#version 440 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Texcoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec4 Tangent;

layout (std140) uniform Matrices //#binding 0
{
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

out vec2 PS_IN_Texcoord;

void main()
{
	gl_Position = Projection * View * Model * vec4(Position, 1.0f);
	PS_IN_Texcoord = Texcoord;
}