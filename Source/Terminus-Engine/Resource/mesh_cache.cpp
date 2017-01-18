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
            
            Mesh* mesh = mesh_factory::create(id);
            
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
