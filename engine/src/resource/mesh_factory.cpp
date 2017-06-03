#include <resource/mesh_factory.h>
#include <resource/tsm_loader.h>
#include <core/context.h>
#include <utility/profiler.h>

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
            uint32_t index_buffer_size;
            uint32_t vertex_buffer_size;
            BufferUsageType usageType;
        };
        
        void create_mesh_task(void* data)
        {
            RenderDevice& device = context::get_render_device();
            CreateMeshTaskData* task_data = (CreateMeshTaskData*)data;
            
            BufferCreateDesc desc;
            
            desc.data = task_data->index_buffer_data;
            desc.size = task_data->index_buffer_size;
            desc.usage_type = task_data->usageType;
            
            IndexBuffer* indexBuffer = device.create_index_buffer(desc);
            
            if(!indexBuffer)
            {
                return;
            }
            
            desc.data = task_data->vertex_buffer_data;
            desc.size = task_data->vertex_buffer_size;
            
            VertexBuffer* vertexBuffer = device.create_vertex_buffer(desc);
            
            if(!vertexBuffer)
            {
                return;
            }
            
            VertexArrayCreateDesc va_desc;
            
            va_desc.index_buffer = indexBuffer;
            va_desc.vertex_buffer = vertexBuffer;
            va_desc.layout_type = task_data->layoutType;
            
            task_data->mesh->vertex_array = device.create_vertex_array(va_desc);
            
            if(!task_data->mesh->vertex_array)
            {
                return;
            }
        }
        
        Mesh* create(String mesh_name)
        {
            asset_common::MeshLoadData* data = tsm_loader::load(mesh_name);
            
            if(!data)
                return nullptr;
            
            Renderer* renderer = &context::get_renderer();
			Task* task = renderer->create_upload_task();
            CreateMeshTaskData* gpu_task_data = task_data<CreateMeshTaskData>(task);
            
            Mesh* mesh = new Mesh();
            
            gpu_task_data->mesh = mesh;
            gpu_task_data->index_buffer_data = &data->indices[0];
            gpu_task_data->index_buffer_size = sizeof(uint32_t) * data->header.m_IndexCount;
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
            
            
            task->_function.Bind<&create_mesh_task>();
            
            renderer->enqueue_upload_task(task);
            
            mesh->sub_meshes = new SubMesh[data->header.m_MeshCount];
            mesh->min_extents = data->header.m_MinExtents;
            mesh->max_extents = data->header.m_MaxExtents;
            
            for (int i = 0; i < data->header.m_MeshCount; i++)
            {
                mesh->sub_meshes[i].base_index = data->meshes[i].m_BaseIndex;
                mesh->sub_meshes[i].index_count = data->meshes[i].m_IndexCount;
                mesh->sub_meshes[i].base_vertex = data->meshes[i].m_BaseVertex;
                mesh->sub_meshes[i].min_extents = data->meshes[i].m_MinExtents;
                mesh->sub_meshes[i].max_extents = data->meshes[i].m_MaxExtents;
                
                int mat_index = (int)data->meshes[i].m_MaterialIndex;
                String mat_path = std::string(data->materials[mat_index].material);
                
                mesh->sub_meshes[i].material = context::get_material_cache().load(mat_path);
            }
            
            mesh->mesh_count = data->header.m_MeshCount;
            mesh->id = mesh_name;
            
            TE_SAFE_DELETE(data);
            
            return mesh;
        }
    }
} // namespace termnus
