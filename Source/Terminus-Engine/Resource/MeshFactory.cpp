#include "MeshFactory.h"

namespace Terminus { namespace Resource {

		MeshFactory::MeshFactory()
		{
            m_rendering_thread_pool = Global::GetRenderingThreadPool();
		}

		MeshFactory::~MeshFactory()
		{

		}

		void MeshFactory::Initialize(Graphics::RenderDevice* device)
		{
			m_device = device;
		}

		Mesh* MeshFactory::Create(AssetCommon::MeshLoadData* _Data)
		{
            m_mesh_vertex_array = nullptr;
            m_task_sucess = true;
            
            m_task_data.index_buffer_data = &_Data->indices[0];
            m_task_data.index_buffer_size = sizeof(uint) * _Data->header.m_IndexCount;
            m_task_data.usageType = BufferUsageType::STATIC;
            
			if (_Data->IsSkeletal)
			{
                m_task_data.vertex_buffer_data = &_Data->skeletalVertices[0];
                m_task_data.vertex_buffer_size = sizeof(SkeletalVertex) * _Data->header.m_VertexCount;
                m_task_data.layout = nullptr;
                m_task_data.layoutType = InputLayoutType::STANDARD_SKINNED_VERTEX;
			}
			else
			{
                m_task_data.vertex_buffer_data = &_Data->vertices[0];
                m_task_data.vertex_buffer_size = sizeof(Vertex) * _Data->header.m_VertexCount;
                m_task_data.layout = nullptr;
                m_task_data.layoutType = InputLayoutType::STANDARD_VERTEX;
			}
            
            TaskData* task = m_rendering_thread_pool->CreateTask();
            task->data = (void*)&m_task_data;
            task->function.Bind<MeshFactory, &MeshFactory::CreateGPUResourcesTask>(this);
            m_rendering_thread_pool->SubmitAndWait();

            if(m_task_sucess)
            {
                Mesh* mesh = new Mesh();
                mesh->VertexArray = m_mesh_vertex_array;
                return mesh;
            }
            
			return nullptr;
		}
    
        TASK_METHOD_DEFINITION(MeshFactory, CreateGPUResourcesTask)
        {
            MeshGPUResourcesTaskData* task_data = (MeshGPUResourcesTaskData*)data;
            
            Graphics::IndexBuffer* indexBuffer = m_device->CreateIndexBuffer(task_data->index_buffer_data, task_data->index_buffer_size, task_data->usageType);
            
            if(!indexBuffer)
            {
                m_task_sucess = false;
                return;
            }
            
            Graphics::VertexBuffer* vertexBuffer = m_device->CreateVertexBuffer(task_data->vertex_buffer_data, task_data->vertex_buffer_size, task_data->usageType);
            
            if(!vertexBuffer)
            {
                m_task_sucess = false;
                return;
            }
            
            m_mesh_vertex_array = m_device->CreateVertexArray(vertexBuffer, indexBuffer, task_data->layoutType);
            
            if(!m_mesh_vertex_array)
            {
                m_task_sucess = false;
                return;
            }
        }
} }
