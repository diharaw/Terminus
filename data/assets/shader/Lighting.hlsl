
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct PointLight
{
	float3 color;
	float  intensity;
	float3 position;
	float  radius;
};

struct SpotLight
{
	float3 color;
	float  intensity;
	float3 position;
	float3 direction;
};

struct DirectionalLight
{
	float3 color;
	float  intensity;
	float3 direction;
};

cbuffer u_LightUniforms : register (c0)
{
	PointLight 		 point_lights[MAX_POINT_LIGHTS];
	SpotLight  		 spot_lights[MAX_SPOT_LIGHTS];
	DirectionalLight directional_light;
	int 			 point_light_count;
	int 			 spot_light_count;
};