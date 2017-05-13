#pragma once

#include <core/macro.h>
#include <core/types.h>
#include <string>

TERMINUS_BEGIN_NAMESPACE

struct Material;
struct VertexArray;

struct SubMesh
{
    Material* material;
	uint32_t  index_count;
	uint32_t  base_vertex;
	uint32_t  base_index;
	Vector3   max_extents;
	Vector3   min_extents;
};

struct Mesh
{
	std::string  id;
	bool		 is_skeletal;
	uint32_t	 mesh_count;
	SubMesh*	 sub_meshes;
    Vector3		 max_extents;
    Vector3		 min_extents;
	VertexArray* vertex_array;
};

TERMINUS_END_NAMESPACE