#ifdef APPLE
#version 410 core
#else
#version 450 core
#endif

// ------------------------------------------------------------------
// INPUT VARIABLES  -------------------------------------------------
// ------------------------------------------------------------------

layout(location = 0) in vec3  VS_IN_Position;
layout(location = 1) in vec2  VS_IN_TexCoord;
layout(location = 2) in vec3  VS_IN_Normal;
layout(location = 3) in vec4  VS_IN_Tangent;

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

layout (std140) uniform u_PerFrame //#binding 0
{ 
	mat4 u_last_vp_mat;
	mat4 u_vp_mat;
	mat4 u_inv_vp_mat;
	mat4 u_proj_mat;
	mat4 u_view_mat;
	vec3 u_view_pos;
	vec3 u_view_dir;
};

// ------------------------------------------------------------------
// OUTPUT VARIABLES  ------------------------------------------------
// ------------------------------------------------------------------

out vec3 PS_IN_TexCoords;

// ------------------------------------------------------------------
// MAIN  ------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{
	vec4 pos = u_vp_mat * vec4(VS_IN_Position, 1.0f);

	PS_IN_TexCoords = VS_IN_Position;
	gl_Position = pos.xyww;
}