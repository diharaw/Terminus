#pragma once

#include <core/macro.h>
#include <core/types.h>

TERMINUS_BEGIN_NAMESPACE

// Uniform Buffer Structs
struct TE_ALIGNED(16) PerFrameUniforms
{
	Matrix4 last_view_projection;
	Matrix4 view_projection;
	Matrix4 inverse_view_projection;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 view_sky;
	Vector3 view_position;
	Vector3 view_direction;
};

struct TE_ALIGNED(16) PerFrameSkyUniforms
{
	Matrix4 projection;
	Matrix4 view;
};

struct TE_ALIGNED(16) PerDrawUniforms
{
	Matrix4 model_view_projection;
	Matrix4 model;
	Vector3 position;
};

struct TE_ALIGNED(16) PerDrawMaterialUniforms
{
	Vector4 diffuse;
	float   roughness;
	float   metalness;
};

struct TE_ALIGNED(16) PerDrawBoneOffsetUniforms
{
	Matrix4 bone_offset[100];
};

namespace renderer
{
	
}

TERMINUS_END_NAMESPACE