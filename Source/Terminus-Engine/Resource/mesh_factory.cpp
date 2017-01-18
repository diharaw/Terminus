#include <Resource/mesh_factory.h>
#include <Core/context.h>
#include <Utility/Remotery.h>

namespace terminus
{
    namespace mesh_factory
    {
        Mesh* create(AssetCommon::MeshLoadData* _Data)
        {
            TERMINUS_BEGIN_CPU_PROFILE(CreateMesh);
            
            Task task;
            CreateMeshTaskData* data = task_data<CreateMeshTaskData>(task);
            
            Mesh* mesh = new Mesh();
            
            data->mesh = mesh;
            data->index_buffer_data = &_Data->indices[0];
            data->index_buffer_size = sizeof(uint) * _Data->header.m_IndexCount;
            data->usageType = BufferUsageType::STATIC;
            
            if (_Data->IsSkeletal)
            {
                data->vertex_buffer_data = &_Data->skeletalVertices[0];
                data->vertex_buffer_size = sizeof(SkeletalVertex) * _Data->header.m_VertexCount;
                data->layout = nullptr;
                data->layoutType = InputLayoutType::STANDARD_SKINNED_VERTEX;
            }
            else
            {
                data->vertex_buffer_data = &_Data->vertices[0];
                data->vertex_buffer_size = sizeof(Vertex) * _Data->header.m_VertexCount;
                data->layout = nullptr;
                data->layoutType = InputLayoutType::STANDARD_VERTEX;
            }
            
            
            task._function.Bind<&create_mesh_task>();
            Global::get_context()._rendering_thread.enqueue_upload_task(task);
            
            
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
            
            //T_SAFE_DELETE(_Data);
            
            TERMINUS_END_CPU_PROFILE;
            
            return mesh;
        }
        
        void create_mesh_task(void* data)
        {
            RenderDevice& device = context::get_render_device();
            CreateMeshTaskData* task_data = (CreateMeshTaskData*)data;
            
            IndexBuffer* indexBuffer = device.CreateIndexBuffer(task_data->index_buffer_data, task_data->index_buffer_size, task_data->usageType);
            
            if(!indexBuffer)
            {
                return;
            }
            
            VertexBuffer* vertexBuffer = device.CreateVertexBuffer(task_data->vertex_buffer_data, task_data->vertex_buffer_size, task_data->usageType);
            
            if(!vertexBuffer)
            {
                return;
            }
            
            task_data->mesh->VertexArray = device.CreateVertexArray(vertexBuffer, indexBuffer, task_data->layoutType);
            
            if(!task_data->mesh->VertexArray)
            {
                return;
            }
        }
    }
} // namespace termnus
