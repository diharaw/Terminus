// ------------------------------------------------------------------
// UNIFORM BUFFERS --------------------------------------------------
// ------------------------------------------------------------------

cbuffer u_PerFrame : register(b0)
{
	float4x4 LastViewProjection;
	float4x4 ViewProjection;
	float4x4 InverseViewProjection;
	float4x4 Projection;
	float4x4 View;
	float3   ViewPosition;
	float3   ViewDirection;
};

cbuffer u_PerEntity : register(b1)
{
	float4x4 ModelViewProjection;
	float4x4 Model;	
	float3   Position;
};

#ifdef SKELETAL_VERTEX
cbuffer u_Bones : register(b2)
{
	float4x4 BoneOffset[100];
};
#endif

// ------------------------------------------------------------------
// INPUT VARIABLES  -------------------------------------------------
// ------------------------------------------------------------------

struct VertexInputType
{
	float4 VS_IN_Position : POSITION;
	float2 VS_IN_TexCoord : TEXCOORD0;
	float3 VS_IN_Normal   : NORMAL;
	float4 VS_IN_Tangent  : TANGENT;
#ifdef SKELETAL_VERTEX
	int4   VS_IN_BoneIndices;
	float4 VS_IN_BoneWeights;
#endif
};

// ------------------------------------------------------------------
// OUTPUT VARIABLES  ------------------------------------------------
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
// MAIN  ------------------------------------------------------------
// ------------------------------------------------------------------

PixelInputType VertexMain(VertexInputType input)
{
	PixelInputType output;

	input.PS_IN_Position.w = 1.0f;

#ifdef SKELETAL_VERTEX
	float4x4 BoneTransform  =  mul(BoneOffset[input.VS_IN_BoneIndices[0]] , input.VS_IN_BoneWeights[0]);
    BoneTransform 	        += mul(BoneOffset[input.VS_IN_BoneIndices[1]] , input.VS_IN_BoneWeights[1]);
    BoneTransform 	   		+= mul(BoneOffset[input.VS_IN_BoneIndices[2]] , input.VS_IN_BoneWeights[2]);
    BoneTransform 	   		+= mul(BoneOffset[input.VS_IN_BoneIndices[3]] , input.VS_IN_BoneWeights[3]);

    output.PS_IN_Position = mul(BoneTransform, input.VS_IN_Position);
	output.PS_IN_Position = mul(ModelViewProjection, output.PS_IN_Position);
#else
	output.PS_IN_Position = mul(ModelViewProjection, input.VS_IN_Position);
#endif
	
	output.PS_IN_TexCoords = input.VS_IN_TexCoord;

#ifdef USE_NORMAL_MAP
	float3 bitangent = cross(input.VS_IN_Normal, input.VS_IN_Tangent);
	float3 T = normalize(vec3(Model * vec4(VS_IN_Tangent,   0.0)));
   	float3 B = normalize(vec3(Model * vec4(bitangent, 0.0)));
   	float3 N = normalize(vec3(Model * vec4(VS_IN_Normal,    0.0)));
   	output.PS_IN_TBN = float3x3(T, B, N)
#endif

	return output;
}