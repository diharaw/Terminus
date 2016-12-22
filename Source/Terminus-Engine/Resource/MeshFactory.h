#pragma once

#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Mesh.h"
#include "../Graphics/RenderDevice.h"
#include "../Global.h"
#include <string>

namespace Terminus { namespace Resource {

    struct MeshGPUResourcesTaskData
    {
        Graphics::VertexBuffer* vertexBuffer;
        Graphics::IndexBuffer* indexBuffer;
        InputLayoutType layoutType;
        InputLayout* layout;
        void* index_buffer_data;
        void* vertex_buffer_data;
        uint index_buffer_size;
        uint vertex_buffer_size;
        BufferUsageType usageType;
    };
    
	class MeshFactory
	{
	private:
		Graphics::RenderDevice* m_device;
        Graphics::VertexArray* m_mesh_vertex_array;
        ThreadPool* m_rendering_thread_pool;
        
        MeshGPUResourcesTaskData m_task_data;
        bool m_task_sucess;

	public:
		MeshFactory();
		~MeshFactory();
		void Initialize(Graphics::RenderDevice* device);
		Mesh* Create(AssetCommon::MeshLoadData* _Data);
        
    private:
        TASK_METHOD_DECLARATION(CreateGPUResourcesTask);
	};

} }

#endif
