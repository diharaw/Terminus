#include <Resource/mesh_factory.h>
#include <Resource/tsm_loader.h>
#include <Core/context.h>
#include <Utility/Remotery.h>

namespace terminus
{
    namespace mesh_factory
    {
        struct CreateMeshTaskData
        {
            Mesh* mesh;
            VertexBuffer* vertexBuffer;
            IndexBuffer* indexBuffer;
            InputLayoutType layoutType;
            InputLayout* layout;
            void* index_buffer_data;
            void* vertex_buffer_data;
            uint index_buffer_size;
            uint vertex_buffer_size;
            BufferUsageType usageType;
        };
        
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
        
        Mesh* create(String mesh_name)
        {
            TERMINUS_BEGIN_CPU_PROFILE(CreateMesh);
            
            asset_common::MeshLoadData* data = tsm_loader::load(mesh_name);
            
            if(!data)
                return nullptr;
            
            Task task;
            CreateMeshTaskData* gpu_task_data = task_data<CreateMeshTaskData>(task);
            
            Mesh* mesh = new Mesh();
            
            gpu_task_data->mesh = mesh;
            gpu_task_data->index_buffer_data = &data->indices[0];
            gpu_task_data->index_buffer_size = sizeof(uint) * data->header.m_IndexCount;
            gpu_task_data->usageType = BufferUsageType::STATIC;
            
            if (data->IsSkeletal)
            {
                gpu_task_data->vertex_buffer_data = &data->skeletalVertices[0];
                gpu_task_data->vertex_buffer_size = sizeof(SkeletalVertex) * data->header.m_VertexCount;
                gpu_task_data->layout = nullptr;
                gpu_task_data->layoutType = InputLayoutType::STANDARD_SKINNED_VERTEX;
            }
            else
            {
                gpu_task_data->vertex_buffer_data = &data->vertices[0];
                gpu_task_data->vertex_buffer_size = sizeof(Vertex) * data->header.m_VertexCount;
                gpu_task_data->layout = nullptr;
                gpu_task_data->layoutType = InputLayoutType::STANDARD_VERTEX;
            }
            
            
            task._function.Bind<&create_mesh_task>();
            
            Context& context = Global::get_context();
            
            submit_gpu_upload_task(task);
            
            mesh->SubMeshes = new SubMesh[data->header.m_MeshCount];
            mesh->m_MinExtents = data->header.m_MinExtents;
            mesh->m_MaxExtents = data->header.m_MaxExtents;
            
            for (int i = 0; i < data->header.m_MeshCount; i++)
            {
                mesh->SubMeshes[i].m_BaseIndex = data->meshes[i].m_BaseIndex;
                mesh->SubMeshes[i].m_IndexCount = data->meshes[i].m_IndexCount;
                mesh->SubMeshes[i].m_BaseVertex = data->meshes[i].m_BaseVertex;
                mesh->SubMeshes[i].m_MinExtents = data->meshes[i].m_MinExtents;
                mesh->SubMeshes[i].m_MaxExtents = data->meshes[i].m_MaxExtents;
                
                int mat_index = (int)data->meshes[i].m_MaterialIndex;
                String mat_path = std::string(data->materials[mat_index].material);
                
                mesh->SubMeshes[i]._material = context::get_material_cache().load(mat_path);
            }
            
            mesh->MeshCount = data->header.m_MeshCount;
            mesh->id = mesh_name;
            
            T_SAFE_DELETE(data);
            
            TERMINUS_END_CPU_PROFILE;
            
            return mesh;
        }
    }
} // namespace termnus
