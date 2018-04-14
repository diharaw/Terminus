#pragma once

#include "tsm_headers.h"
#include "tsm_material.h"
#include "tsm_vertex.h"

struct AssimpImportData
{
	TSM_FileHeader      header;
	TSM_MeshHeader*     meshes;
	Assimp_Material*    materials;
	TSM_Vertex*			vertices;
	TSM_SkeletalVertex* skeletal_vertices;
	uint*			    indices;
	bool			    skeletal;
    std::string         mesh_path;
    std::string         filename;
};

namespace assimp_importer
{
	extern AssimpImportData* import_mesh(const char* file);
}
