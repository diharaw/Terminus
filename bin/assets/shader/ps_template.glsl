#ifdef APPLE
#version 410 core
#else
#version 450 core
#endif

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

#ifndef USE_DIFFUSE_MAP
layout (std140) uniform u_Material
{ 
	vec4  diffuse;
	float roughness;
	float metalness;
};
#endif

layout (std140) uniform u_PointLight
{ 
	int   count;
	vec3  position[10];
	vec3  color[10];
	float radius[10];
};

layout (std140) uniform u_SpotLight
{ 
	int   count;
	vec3  position[10];
	vec3  direction[10];
	vec3  color[10];
};

layout (std140) uniform u_DirectionalLight
{ 
	vec3  direction;
	vec3  color;
};

// ------------------------------------------------------------------
// UNIFORM SAMPLERS -------------------------------------------------
// ------------------------------------------------------------------

#ifdef USE_DIFFUSE_MAP
uniform sampler u_Diffuse;
#endif

#ifdef USE_NORMAL_MAP
uniform sampler u_Normal;
#endif

#ifdef USE_ROUGHNESS_MAP
uniform sampler u_Roughness;
#endif

#ifdef USE_METALNESS_MAP
uniform sampler u_Metalness;
#endif

#ifdef USE_PARALLAX_OCCLUSION
uniform sampler u_Displacement;
#endif

// ------------------------------------------------------------------
// INPUT VARIABLES  -------------------------------------------------
// ------------------------------------------------------------------

in vec2 PS_IN_TexCoords;
in vec3 PS_IN_Position;
in vec3 PS_IN_Normal;

#ifdef USE_NORMAL_MAP
out mat3 PS_IN_TBN;	
#endif

// ------------------------------------------------------------------
// OUTPUT VARIABLES  ------------------------------------------------
// ------------------------------------------------------------------

out vec4 PS_OUT_Color;

// ------------------------------------------------------------------
// FUNCTIONS  -------------------------------------------------------
// ------------------------------------------------------------------

vec2 GetTexCoords()
{
#ifdef USE_PARALLAX_OCCLUSION
	vec2 parallax_coords;
	return parallax_coords;
#else
	return PS_IN_TexCoords;
#endif
}

vec4 GetDiffuse(vec2 texCoords)
{
#ifndef USE_DIFFUSE_MAP
	return u_Material.diffuse;
#else
	return texture(u_Diffuse, texCoords);
#endif
}

vec3 GetNormal(vec2 texCoords)
{
#ifndef USE_NORMAL_MAP
	return PS_IN_Normal;
#else
	return texture(u_Normal, texCoords).rgb;
#endif
}

float GetRoughness(vec2 texCoords)
{
#ifndef USE_ROUGHNESS_MAP
	return u_Material.roughness;
#else
	return texture(u_Roughness, texCoords).r;
#endif
}

float GetMetalness(vec2 texCoords)
{
#ifndef USE_METALNESS_MAP
	return u_Material.metalness;
#else
	return texture(u_Metalness, texCoords).r;
#endif
}

// ------------------------------------------------------------------
// MAIN -------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{
	vec2 texCoords = GetTexCoords();
	PS_OUT_Color = GetDiffuse(texCoords);
}