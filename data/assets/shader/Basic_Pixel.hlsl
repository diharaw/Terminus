Texture2D diffuseTexture : register(t0);
SamplerState testSampler : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
};

float4 PixelMain(PixelInputType input) : SV_TARGET
{
	float4 finalColor;

	//finalColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	finalColor = diffuseTexture.Sample(testSampler, input.texcoord);

	return finalColor;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}