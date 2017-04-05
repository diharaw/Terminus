#include <resource/mesh_cache.h>
#include <resource/tsm_loader.h>
#include <core/context.h>
#include <io/filesystem.h>

#include <iostream>

namespace terminus
{
    struct MeshUnloadTaskData
    {
        Mesh* _mesh;
    };
    
    void mesh_unload_task(void* data)
    {
        MeshUnloadTaskData* task_data = (MeshUnloadTaskData*)data;
        RenderDevice& device = context::get_render_device();
        
        device.destroy_vertex_array(task_data->_mesh->VertexArray);
    }
    
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
            Task task;
            
            MeshUnloadTaskData* data = task_data<MeshUnloadTaskData>(task);
            data->_mesh = mesh;
            task._function.Bind<&mesh_unload_task>();
            
            submit_gpu_upload_task(task);
            
            MaterialCache& mat_cache = context::get_material_cache();
            
            for(int i = 0; i < mesh->MeshCount; i++)
            {
                mat_cache.unload(mesh->SubMeshes[i]._material);
            }
            
			T_SAFE_DELETE_ARRAY(mesh->SubMeshes);
			T_SAFE_DELETE(mesh);
		}
	}
}
