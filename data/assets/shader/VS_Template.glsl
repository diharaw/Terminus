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
layout(location = 3) in vec3  VS_IN_Tangent;

#ifdef SKELETAL_VERTEX
layout(location = 4) in ivec4 VS_IN_BoneIndices;
layout(location = 5) in vec4  VS_IN_BoneWeights;
#endif

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

layout (std140) uniform u_PerFrame
{ 
	mat4 LastViewProjection;
	mat4 ViewProjection;
	mat4 InverseViewProjection;
	mat4 Projection;
	mat4 View;
	vec3 ViewPosition;
	vec3 ViewDirection;
};

layout (std140) uniform u_PerEntity
{ 
	mat4 ModelViewProjection;
	mat4 Model;	
	vec3 Position;
};

#ifdef SKELETAL_VERTEX
layout (std140) uniform u_Bones
{
	mat4 BoneOffset[100];
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
	mat4 BoneTransform  = u_Bones.BoneOffset[VS_IN_BoneIndices[0]] * VS_IN_BoneWeights[0];
    BoneTransform 	   += u_Bones.BoneOffset[VS_IN_BoneIndices[1]] * VS_IN_BoneWeights[1];
    BoneTransform 	   += u_Bones.BoneOffset[VS_IN_BoneIndices[2]] * VS_IN_BoneWeights[2];
    BoneTransform 	   += u_Bones.BoneOffset[VS_IN_BoneIndices[3]] * VS_IN_BoneWeights[3];
	vec4 pos = BoneTransform * vec4(VS_IN_Position, 1.0f);
	pos = u_PerEntity.ModelViewProjection * pos;
#else
	vec4 pos = u_PerEntity.ModelViewProjection * vec4(VS_IN_Position, 1.0f);
#endif

	PS_IN_Position = pos.xyz;
	gl_Position = pos;

	PS_IN_Normal = VS_IN_Normal;

#ifdef USE_NORMAL_MAP
	vec3 bitangent = cross(VS_IN_Normal, VS_IN_Tangent);
	vec3 T = normalize(vec3(u_PerEntity.Model * vec4(VS_IN_Tangent,   0.0)));
   	vec3 B = normalize(vec3(u_PerEntity.Model * vec4(bitangent, 0.0)));
   	vec3 N = normalize(vec3(u_PerEntity.Model * vec4(VS_IN_Normal,    0.0)));
   	PS_IN_TBN = mat3(T, B, N)
#endif

}