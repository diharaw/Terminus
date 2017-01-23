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

#ifndef QUAD_VERTEX
layout(location = 2) in vec3  VS_IN_Normal;
layout(location = 3) in vec3  VS_IN_Tangent;
#endif

#ifdef SKELETAL_VERTEX
layout(location = 4) in ivec4 VS_IN_BoneIndices;
layout(location = 5) in vec4  VS_IN_BoneWeights;
#endif

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

layout (std140) uniform u_PerFrame
{ 
	mat4 u_last_vp_mat;
	mat4 u_vp_mat;
	mat4 u_inv_vp_mat;
	mat4 u_proj_mat;
	mat4 u_view_mat;
	vec3 u_view_pos;
	vec3 u_view_dir;
};

layout (std140) uniform u_PerEntity
{ 
	mat4 u_mvp_mat;
	mat4 u_model_mat;	
	vec3 u_pos;
};

#ifdef SKELETAL_VERTEX
layout (std140) uniform u_Bones
{
	mat4 u_bone_offsets[100];
};
#endif

// ------------------------------------------------------------------
// OUTPUT VARIABLES  ------------------------------------------------
// ------------------------------------------------------------------

out vec2 PS_IN_TexCoords;
out vec3 PS_IN_Position;
out vec3 PS_IN_Normal;

#ifdef USE_NORMAL_MAP
out mat3 PS_IN_TBN;	
#endif

// ------------------------------------------------------------------
// MAIN  ------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{
	
#ifdef SKELETAL_VERTEX
	mat4 bone_transform  = u_bone_offsets[VS_IN_BoneIndices[0]] * VS_IN_BoneWeights[0];
    bone_transform 	    += u_bone_offsets[VS_IN_BoneIndices[1]] * VS_IN_BoneWeights[1];
    bone_transform 	    += u_bone_offsets[VS_IN_BoneIndices[2]] * VS_IN_BoneWeights[2];
    bone_transform 	    += u_bone_offsets[VS_IN_BoneIndices[3]] * VS_IN_BoneWeights[3];
	vec4 pos = bone_transform * vec4(VS_IN_Position, 1.0f);
	pos = u_mvp_mat * pos;
#else
	vec4 pos = u_mvp_mat * vec4(VS_IN_Position, 1.0f);
#endif

	PS_IN_Position = pos.xyz;
	gl_Position = pos;

	PS_IN_TexCoords = VS_IN_TexCoord;
	PS_IN_Normal = VS_IN_Normal;

#ifdef USE_NORMAL_MAP
	vec3 bitangent = cross(VS_IN_Normal, VS_IN_Tangent);
	vec3 T = normalize(vec3(u_model_mat * vec4(VS_IN_Tangent,   0.0)));
   	vec3 B = normalize(vec3(u_model_mat * vec4(bitangent, 0.0)));
   	vec3 N = normalize(vec3(u_model_mat * vec4(VS_IN_Normal,    0.0)));
   	PS_IN_TBN = mat3(T, B, N);
#endif

}