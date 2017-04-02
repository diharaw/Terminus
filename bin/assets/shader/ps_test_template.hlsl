
// ------------------------------------------------------------------
// INPUT VARIABLES  -------------------------------------------------
// ------------------------------------------------------------------

struct PixelInputType
{
	float4 PS_IN_Position  : SV_POSITION;
	float2 PS_IN_TexCoords : TEXCOORD0;
	float3 PS_IN_Normal    : NORMAL;
	float4 PS_IN_Tangent   : TANGENT;
#ifdef SKELETAL_VERTEX
	int4   PS_IN_BoneIndices;
	float4 PS_IN_BoneWeights;
#endif
};

// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

cbuffer u_Material : register(b0)
{ 
	float4 u_diffuse_value;
	float  u_roughness_value;
	float  u_metalness_value;
};

cbuffer u_PointLight : register(b1)
{ 
	int    u_pl_count;
	float3 u_pl_position[10];
	float3 u_pl_color[10];
	float  u_pl_radius[10];
};

cbuffer u_SpotLight : register(b2)
{ 
	int    u_sl_count;
	float3 u_sl_position[10];
	float3 u_sl_direction[10];
	float3 u_sl_color[10];
};

cbuffer u_DirectionalLight : register(b3)
{ 
	float3 u_dl_direction;
	float3 u_dl_color;
};

// ------------------------------------------------------------------
// UNIFORM SAMPLERS -------------------------------------------------
// ------------------------------------------------------------------

#ifdef USE_DIFFUSE_MAP
Texture2D u_diffuse : register(t0);
SamplerState u_diffuse_sampler : register(s0);
#endif

#ifdef USE_NORMAL_MAP
Texture2D u_normal : register(t1);
SamplerState u_normal_sampler : register(s1);
#endif

#ifdef USE_ROUGHNESS_MAP
Texture2D u_roughness : register(t2);
SamplerState u_roughness_sampler : register(s2);
#endif

#ifdef USE_METALNESS_MAP
Texture2D u_metalness : register(t3);
SamplerState u_metalness_sampler : register(s3);
#endif

#ifdef USE_PARALLAX_OCCLUSION
Texture2D u_displacement : register(t4);
SamplerState u_displacement_sampler : register(s4);
#endif

// ------------------------------------------------------------------
// FUNCTIONS  -------------------------------------------------------
// ------------------------------------------------------------------

float2 GetTexCoords(PixelInputType input)
{
#ifdef USE_PARALLAX_OCCLUSION
	float2 parallax_coords;
	return parallax_coords;
#else
	return input.PS_IN_TexCoords;
#endif
}

float4 GetDiffuse(float2 texCoords)
{
#ifndef USE_DIFFUSE_MAP
	return u_diffuse_value;
#else
	return u_diffuse.Sample(u_diffuse_sampler, texCoords);
#endif
}

float3 GetNormal(PixelInputType input, float2 texCoords)
{
#ifndef USE_NORMAL_MAP
	return input.PS_IN_Normal;
#else
	return u_normal.Sample(u_normal_sampler, texCoords).rgb;
#endif
}

float GetRoughness(float2 texCoords)
{
#ifndef USE_ROUGHNESS_MAP
	return u_roughness_value;
#else
	return u_roughness.Sample(u_roughness_sampler, texCoords).r;
#endif
}

float GetMetalness(float2 texCoords)
{
#ifndef USE_METALNESS_MAP
	return u_metalness_value;
#else
	return u_metalness.Sample(u_metalness_sampler, texCoords).r;
#endif
}

float4 GetFinalColor(PixelInputType input)
{
	float2 texCoords = GetTexCoords(input);
	float3 test_normal = GetNormal(input, texCoords);
	float4 color = GetDiffuse(texCoords);
	return color;
}

// ------------------------------------------------------------------
// MAIN  ------------------------------------------------------------
// ------------------------------------------------------------------

float4 PixelMain(PixelInputType input) : SV_TARGET
{
	float4 final_color = GetFinalColor(input);

	final_color.w = 1.0f;
#ifdef ALPHA_DISCARD
	if(final_color.w < 0.1)
		discard;
#endif
	
	return final_color;
}