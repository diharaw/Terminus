#ifdef APPLE
#version 410 core
#else
#version 450 core
#endif

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

layout (std140) uniform u_Material
{ 
	vec4  u_diffuse_value;
	float u_roughness_value;
	float u_metalness_value;
};

layout (std140) uniform u_PointLight
{ 
	int   u_pl_count;
	vec3  u_pl_position[10];
	vec3  u_pl_color[10];
	float u_pl_radius[10];
};

layout (std140) uniform u_SpotLight
{ 
	int   u_sl_count;
	vec3  u_sl_position[10];
	vec3  u_sl_direction[10];
	vec3  u_sl_color[10];
};

layout (std140) uniform u_DirectionalLight
{ 
	vec3  u_dl_direction;
	vec3  u_dl_color;
};

// ------------------------------------------------------------------
// UNIFORM SAMPLERS -------------------------------------------------
// ------------------------------------------------------------------

#ifdef USE_DIFFUSE_MAP
uniform sampler2D u_Diffuse;
#endif

#ifdef USE_NORMAL_MAP
uniform sampler2D u_Normal;
#endif

#ifdef USE_ROUGHNESS_MAP
uniform sampler2D u_Roughness;
#endif

#ifdef USE_METALNESS_MAP
uniform sampler2D u_Metalness;
#endif

#ifdef USE_PARALLAX_OCCLUSION
uniform sampler2D u_Displacement;
#endif

// ------------------------------------------------------------------
// INPUT VARIABLES  -------------------------------------------------
// ------------------------------------------------------------------

in vec2 PS_IN_TexCoords;
in vec3 PS_IN_Position;
in vec3 PS_IN_Normal;

#ifdef USE_NORMAL_MAP
in mat3 PS_IN_TBN;	
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
	return u_diffuse_value;
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
	return u_roughness_value;
#else
	return texture(u_Roughness, texCoords).r;
#endif
}

float GetMetalness(vec2 texCoords)
{
#ifndef USE_METALNESS_MAP
	return u_metalness_value;
#else
	return texture(u_Metalness, texCoords).r;
#endif
}

vec4 GetFinalColor()
{
	vec2 texCoords = GetTexCoords();
	vec4 color = GetDiffuse(texCoords);
	return color;
}

// ------------------------------------------------------------------
// MAIN -------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{
	vec4 final_color = GetFinalColor();

#ifdef ALPHA_DISCARD
	if(final_color.a < 0.1)
		discard;
#endif
	
	PS_OUT_Color = final_color;
}