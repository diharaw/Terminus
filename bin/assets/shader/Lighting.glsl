
#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

struct PointLight
{
	vec3  color;
	float intensity;
	vec3  position;
	float radius;
};

struct SpotLight
{
	vec3  color;
	float intensity;
	vec3  position;
	vec3  direction;
};

struct DirectionalLight
{
	vec3  color;
	float intensity;
	vec3  direction;
};

layout (std140) uniform u_LightUniforms
{
	PointLight 		 point_lights[MAX_POINT_LIGHTS];
	SpotLight  		 spot_lights[MAX_SPOT_LIGHTS];
	DirectionalLight directional_light;
	int 			 point_light_count;
	int 			 spot_light_count;
};