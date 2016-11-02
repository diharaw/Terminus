#pragma once

#ifndef MESH_H
#define MESH_H

#include "../Types.h"
#include "AssetCommon.h"
#include "../Graphics/RenderCommon.h"

struct VertexArray;

struct SubMesh
{
	uint32 m_IndexCount;
	uint32 m_BaseVertex;
	uint32 m_BaseIndex;
	Vector3 m_MaxExtents;
	Vector3 m_MinExtents;
};

struct Mesh
{
	bool IsSkeletal;
	uint MeshCount;
	SubMesh* SubMeshes;
	ResourceHandle VAOHandle;
};

#endif
