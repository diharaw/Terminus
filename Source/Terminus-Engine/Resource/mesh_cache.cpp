#include <Resource/mesh_cache.h>
#include <Resource/tsm_loader.h>
#include <Core/context.h>
#include <IO/filesystem.h>

#include <iostream>

namespace terminus
{
	MeshCache::MeshCache()
	{
        filesystem::add_directory("assets/mesh");
	}

	MeshCache::~MeshCache()
	{
        
	}

	Mesh* MeshCache::load(std::string id)
	{
		if (m_AssetMap.find(id) == m_AssetMap.end())
		{
			std::cout << "Asset not in cache, loading" << std::endl;

			std::string extension = filesystem::get_file_extention(id);

            asset_common::MeshLoadData* data = tsm_loader::load(id);
            
            SubMesh* submeshes = new SubMesh[data->header.m_MeshCount];
            
            for (int i = 0; i < data->header.m_MeshCount; i++)
            {
                submeshes[i].m_BaseIndex = data->meshes[i].m_BaseIndex;
                submeshes[i].m_BaseVertex = data->meshes[i].m_BaseVertex;
                submeshes[i].m_IndexCount = data->meshes[i].m_IndexCount;
                
                int mat_index = (int)data->meshes[i].m_MaterialIndex;
                String mat_path = std::string(data->materials[mat_index].material);
                
                submeshes[i]._material = context::get_material_cache().load(mat_path);
            }
            
            int mesh_count = data->header.m_MeshCount;
            
            Mesh* mesh = mesh_factory::create(data);
            mesh->SubMeshes = submeshes;
            
            mesh->MeshCount = mesh_count;
            mesh->id = id;
            m_AssetMap[id] = mesh;
            
            std::cout << "Asset successfully loaded" << std::endl;
            
            return mesh;
		}
		else
		{
			std::cout << "Asset already in cache, returning reference." << std::endl;
			return m_AssetMap[id];
		}
	}

	void MeshCache::unload(Mesh* mesh)
	{
		if (m_AssetMap.find(mesh->id) != m_AssetMap.end())
		{
            context::get_render_device().DestroyVertexArray(mesh->VertexArray);
			T_SAFE_DELETE_ARRAY(mesh->SubMeshes);
			T_SAFE_DELETE(mesh);
		}
	}
}
