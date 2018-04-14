#pragma once 

#include "types.h"

struct TSM_Vertex
{
	Vector3 position;
	Vector2 tex_coord;
	Vector3 normal;
	Vector3 tangent;
};

struct TSM_SkeletalVertex
{
	Vector3  position;
	Vector2  tex_coord;
	Vector3  normal;
	Vector3  tangent;
	iVector4 bone_indices;
	Vector4  bone_weights;
};