#pragma once

#include <core/types.h>
#include <resource/asset_common.h>
#include <graphics/render_device.h>
#include <graphics/render_common.h>
#include <graphics/material.h>
#include <string>

struct VertexArray;

struct SubMesh
{
    terminus::Material* _material;
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
	terminus::VertexArray* VertexArray;
};
