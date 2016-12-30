#pragma once

#ifndef MESH_H
#define MESH_H

#include "../Types.h"
#include "AssetCommon.h"
#include "../Graphics/RenderDevice.h"
#include "../Graphics/RenderCommon.h"
#include "../Graphics/Material.h"
#include <string>

struct VertexArray;

struct SubMesh
{
    Terminus::Graphics::Material* _material;
	uint32 m_IndexCount;
	uint32 m_BaseVertex;
	uint32 m_BaseIndex;
	Vector3 m_MaxExtents;
	Vector3 m_MinExtents;
};

struct Mesh
{
	std::string id;
	bool IsSkeletal;
	uint MeshCount;
	SubMesh* SubMeshes;
    Vector3 m_MaxExtents;
    Vector3 m_MinExtents;
	Terminus::Graphics::VertexArray* VertexArray;
};

#endif
