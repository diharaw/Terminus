#include "MeshFactory.h"
#include <Core/context.h>

namespace terminus
{
		MeshFactory::MeshFactory()
		{
            m_rendering_thread_pool = Global::GetRenderingThreadPool();
		}

		MeshFactory::~MeshFactory()
		{

		}

		void MeshFactory::Initialize()
		{
			
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
                mesh->SubMeshes = new SubMesh[_Data->header.m_MeshCount];
                mesh->m_MinExtents = _Data->header.m_MinExtents;
                mesh->m_MaxExtents = _Data->header.m_MaxExtents;
                
                for (int i = 0; i < _Data->header.m_MeshCount; i++)
                {
                    mesh->SubMeshes[i].m_BaseIndex = _Data->meshes[i].m_BaseIndex;
                    mesh->SubMeshes[i].m_IndexCount = _Data->meshes[i].m_IndexCount;
                    mesh->SubMeshes[i].m_BaseVertex = _Data->meshes[i].m_BaseVertex;
                    mesh->SubMeshes[i].m_MinExtents = _Data->meshes[i].m_MinExtents;
                    mesh->SubMeshes[i].m_MaxExtents = _Data->meshes[i].m_MaxExtents;
                }
                
                mesh->VertexArray = m_mesh_vertex_array;
                T_SAFE_DELETE(_Data);
                
                return mesh;
            }
            
			return nullptr;
		}
    
        TASK_METHOD_DEFINITION(MeshFactory, CreateGPUResourcesTask)
        {
            RenderDevice& device = context::get_render_device();
            MeshGPUResourcesTaskData* task_data = (MeshGPUResourcesTaskData*)data;
            
            IndexBuffer* indexBuffer = device.CreateIndexBuffer(task_data->index_buffer_data, task_data->index_buffer_size, task_data->usageType);
            
            if(!indexBuffer)
            {
                m_task_sucess = false;
                return;
            }
            
            VertexBuffer* vertexBuffer = device.CreateVertexBuffer(task_data->vertex_buffer_data, task_data->vertex_buffer_size, task_data->usageType);
            
            if(!vertexBuffer)
            {
                m_task_sucess = false;
                return;
            }
            
            m_mesh_vertex_array = device.CreateVertexArray(vertexBuffer, indexBuffer, task_data->layoutType);
            
            if(!m_mesh_vertex_array)
            {
                m_task_sucess = false;
                return;
            }
        }
} // namespace termnus
