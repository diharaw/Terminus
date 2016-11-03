#ifndef AssetCommon_h
#define AssetCommon_h

#include "../Types.h"
#include "../Graphics/RenderCommon.h"

struct Vertex
{
	// Veterx
	Vector3 m_Position;

	// Texture Coodinates
	Vector2 m_TexCoord;

	// Normal
	Vector3 m_Normal;

	// Tangent
	Vector4 m_Tangent;
};

struct SkeletalVertex
{
	Vector3 m_Position;
	Vector2 m_TexCoord;
	Vector3 m_Normal;
	Vector3 m_Tangent;
	iVector4 m_BoneIndices;
	Vector4 m_BoneWeights;
};


typedef struct
{
	uint8 m_MeshType;
	uint16 m_MeshCount;
	uint16 m_MaterialCount;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	Vector3 m_MaxExtents;
	Vector3 m_MinExtents;
	char m_Name[50];
}TSM_FileHeader;

typedef struct
{
	uint8  m_MaterialIndex;
	uint32 m_IndexCount;
	uint32 m_BaseVertex;
	uint32 m_BaseIndex;
	Vector3 m_MaxExtents;
	Vector3 m_MinExtents;
}TSM_MeshHeader;

typedef struct
{
	char m_Albedo[50];
	char m_Normal[50];
	char m_Roughness[50];
	char m_Metallicness[50];
	char m_Displacement[50];
}TSM_Material;

// TSM Skeleton Structs

typedef struct
{
	uint8 m_Index;
	uint8 m_ParentIndex;
	Matrix4 m_OffsetTransform;
}TSM_Joint;

typedef struct
{
	uint16 m_JointCount;
	char m_Name[50];
}TSM_SkeletonHeader;

// TSA Animation Structs

typedef struct
{
	Quaternion m_Rotation;
	Vector3 m_Translation;
	Vector3 m_Scale;
}TSM_Keyframe;

typedef struct
{
	char m_JointName[50];
}TSM_AnimationChannelHeader;

typedef struct
{
	uint16 m_KeyframeCount;
	uint16 m_AnimationChannelCount;
	char m_Name[50];
	double m_Duration;
}TSM_AnimationHeader;

namespace asset_common
{
    struct MeshLoadData
    {
		TSM_FileHeader  header;
		TSM_MeshHeader* meshes;
		TSM_Material*   materials;
		Vertex*			vertices;
		SkeletalVertex* skeletalVertices;
		uint*			indices;
		bool			IsSkeletal;
    };
    
    struct ImageLoadData
    {
        unsigned char* bytes;
        int width;
        int height;
        int bpp;
    };
    
    struct TextLoadData
    {
        char* buffer;
        ShaderType shader_type; // Optional
    };
    
    struct AudioLoadData
    {
        
    };
    
    struct VideoLoadData
    {
        
    };
    
    struct AnimationLoadData
    {
        
    };
}

#endif
