#pragma once

#ifndef MESH_H
#define MESH_H

#include <types.h>
#include <Resource/asset_common.h>
#include <Graphics/render_device.h>
#include <Graphics/render_common.h>
#include <Graphics/material.h>
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

#endif
