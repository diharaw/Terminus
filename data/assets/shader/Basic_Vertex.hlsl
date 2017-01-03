cbuffer MatrixBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 projection;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
};

PixelInputType VertexMain(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(model, input.position);
	output.position = mul(view, output.position);
	output.position = mul(projection, output.position);

	output.texcoord = input.texcoord;

	return output;
}