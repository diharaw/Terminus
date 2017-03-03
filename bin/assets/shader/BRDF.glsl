// ------------------------------------------------------------------
// SPECULAR ---------------------------------------------------------
// ------------------------------------------------------------------

// ------------------------------------------------------------------
// FRESNEL (F) ------------------------------------------------------
// ------------------------------------------------------------------

float FresnelSchlick(float f0, float fd90, float view)
{
	return f0 + (fd90 - f0) * pow(max(1.0 - view, 0.1), 5.0);
}

float Disney(vec3 lightDir, vec3 viewDir, float roughness, vec3 normal)
{
	vec3 halfVector = normalize(lightDir + viewDir);

	float NdotL = max(dot(normal, lightDir), 0.0);
	float LdotH = max(dot(lightDir, halfVector), 0.0);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float energyBias = mix(0.0, 0.5, roughness);
  	float energyFactor = mix(1.0, 1.0 / 1.51, roughness);
  	float fd90 = energyBias + 2.0 * (LdotH * LdotH) * roughness;
  	float f0 = 1.0;

	float lightScatter = FresnelSchlick(f0, fd90, NdotL);
	float viewScatter = FresnelSchlick(f0, fd90, NdotV);

  	return lightScatter * viewScatter * energyFactor;
}

vec3 GGX(vec3 lightDir, vec3 viewDir, float roughness, vec3 metalToSpec, vec3 normal)
{
	vec3 h = normalize(lightDir + viewDir);
	float NdotH = max(dot(normal, h), 0.0);

	float rough2 = max(roughness * roughness, 2.0e-3); // capped so spec highlights don't disappear
	float rough4 = rough2 * rough2;

	float d = (NdotH * rough4 - NdotH) * NdotH + 1.0;
	float D = rough4 / (PI * (d * d));

	// Fresnel
	vec3 reflectivity = metalToSpec;
	float fresnel = 1.0;
	float NdotL = clamp(dot(normal, lightDir), 0.0, 1.0);
	float LdotH = clamp(dot(lightDir, h), 0.0, 1.0);
	float NdotV = clamp(dot(normal, viewDir), 0.0, 1.0);
	vec3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473 * LdotH - 6.98316) * LdotH);

	// geometric / visibility
	float k = rough2 * 0.5;
	float G_SmithL = NdotL * (1.0 - k) + k;
	float G_SmithV = NdotV * (1.0 - k) + k;
	float G = 0.25 / (G_SmithL * G_SmithV);

	return G * D * F;
}

vec3 RadianceIBLIntegration(float NdotV, float roughness, vec3 specular)
{
	vec2 preintegratedFG = texture(u_BRDFIntegrationMap, vec2(roughness, 1.0 - NdotV)).rg;
	return specular * preintegratedFG.r + preintegratedFG.g;
}

vec3 GetSpecular(vec3 lightDir, vec3 viewDir, vec3 normal, float roughness, float metalToSpec)
{
	
}

vec3 GetDiffuse(vec3 lightDir, vec3 viewDir, vec3 normal, float roughness)
{
	
}

vec3 GetAmbient()
{
	
}