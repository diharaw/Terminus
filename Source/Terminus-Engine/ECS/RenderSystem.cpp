#include "RenderSystem.h"
#include "Scene.h"
#include "../Resource/ShaderCache.h"
#include "CameraComponent.h"

namespace Terminus { namespace ECS {
    
    // Sort Method
    
    bool DrawItemSort(Graphics::DrawItem i, Graphics::DrawItem j)
    {
        return i.sort_key.key > j.sort_key.key;
    }

    RenderSystem::RenderSystem()
    {
        m_view_count = 0;
        
        m_thread_pool = Global::GetDefaultThreadPool();
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    void RenderSystem::SetRenderDevice(Graphics::Renderer* renderer)
    {
        m_renderer = renderer;
    }
    
    void RenderSystem::SetShaderCache(Resource::ShaderCache* shaderCache)
    {
        m_shader_cache = shaderCache;
    }
    
    void RenderSystem::Initialize()
    {
        SlotMap<CameraComponent, MAX_COMPONENTS>& component_list = m_scene->GetComponentArray<CameraComponent>();
        
        // For each Scene Camera
        for (int i = 0; i < component_list._num_objects; i++)
        {
            SceneView& view = m_views[m_view_count++];
            view._screen_rect = component_list._objects[i].screen_rect;
            view._is_shadow = false;
            view._cmd_buf_idx = m_renderer->CreateCommandBuffer();
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (Entity entity : m_scene->m_entities)
        {
            if(m_scene->HasComponent(entity, TransformComponent::_id) && m_scene->HasComponent(entity, MeshComponent::_id))
            {
                MeshComponent* mesh_component = (MeshComponent*)m_scene->GetComponent(entity, MeshComponent::_id);
                TransformComponent* transform_component = (TransformComponent*)m_scene->GetComponent(entity, TransformComponent::_id);
                
                Renderable& renderable = m_renderables[m_renderable_count++];
                
                renderable._mesh = mesh_component->mesh;
                renderable._sub_mesh_cull = mesh_component->cull_submeshes;
                // TODO : assign Radius. Add to MeshImporter.
                renderable._transform = transform_component;
                
                if(mesh_component->mesh->IsSkeletal)
                    renderable._type = Graphics::RenderableType::SkeletalMesh;
                else if(mesh_component->mesh->IsSkeletal)
                    renderable._type = Graphics::RenderableType::StaticMesh;
                
                
                // TODO: Create task for generating Shader permutations per view, per renderables
                for (int i = 0; i < renderable._mesh->MeshCount; i++)
                {
                    Resource::ShaderKey key;
                    
                    key.EncodeAlbedo(true);
                    key.EncodeMeshType(renderable._type);
                    key.EncodeNormal(true);
                    key.EncodeMetalness(true);
                    key.EncodeRoughness(true);
                    key.EncodeParallaxOcclusion(false);
                    
                    // Send to rendering thread pool
                    m_shader_cache->Load(key);
                }
                
            }
            
            // Commented for now.
            
//            if(mesh_component && transform_component)
//            {
//                for (auto view : m_views)
//                {
//                    if((view._is_shadow && mesh_component->casts_shadow) || !view._is_shadow)
//                    {
//                        Graphics::DrawItem& draw_item = view._draw_items[view._num_items++];
//                        draw_item.vertex_array = mesh_component->mesh->VertexArray;
//                        
//                        if(mesh_component->mesh->IsSkeletal)
//                            draw_item.type = Graphics::RenderableType::SkeletalMesh;
//                        else
//                            draw_item.type = Graphics::RenderableType::StaticMesh;
//                        
//                        draw_item.transform = &transform_component->global_transform;
//                    }
//                }
//            }
        }
    }
    
    void RenderSystem::Update(double delta)
    {
        m_renderer->GetUniformAllocator()->Clear();
        
        int worker_count = m_thread_pool->WorkerThreadCount();
        LinearAllocator* per_frame_alloc = Global::GetPerFrameAllocator();
        
        // Assign views to threads and frustum cull, sort and fill command buffers in parallel.
        
        int items_per_thread = std::floor((float)m_view_count / (float)worker_count);
        
        if(items_per_thread == 0)
            items_per_thread = m_view_count;
        
        int submitted_items = 0;
        int scene_index = 0;
        bool is_done = false;
        
        while(!is_done)
        {
            int remaining_items = m_view_count - submitted_items;
            
            if(remaining_items <= items_per_thread)
            {
                is_done = true;
                items_per_thread = remaining_items;
            }
            
            TaskData* task = m_thread_pool->CreateTask();
            RenderPrepareTaskData* data = per_frame_alloc->NewPerFrame<RenderPrepareTaskData>();
    
            data->_scene_index = scene_index++;
            
            task->data = data;
            task->function.Bind<RenderSystem, &RenderSystem::RenderPrepareTask>(this);
            
            submitted_items += items_per_thread;
        }
        
        m_thread_pool->SubmitAndWait();
    }
    
    void RenderSystem::Shutdown()
    {
        
    }
    
    void RenderSystem::OnEntityCreated(Entity entity)
    {
        
    }
    
    void RenderSystem::OnEntityDestroyed(Entity entity)
    {
        
    }
    
    TASK_METHOD_DEFINITION(RenderSystem, RenderPrepareTask)
    {
        RenderPrepareTaskData* _data = static_cast<RenderPrepareTaskData*>(data);
        SceneView& scene_view = m_views[_data->_scene_index];
     
        LinearAllocator* uniform_allocator = m_renderer->GetUniformAllocator();
        uniform_allocator->Clear();
        
        // Set up Per frame uniforms
        
        Graphics::PerFrameUniforms* per_frame = uniform_allocator->NewPerFrame<Graphics::PerFrameUniforms>();
        per_frame->projection = *scene_view._projection_matrix;
        per_frame->view       = *scene_view._view_matrix;
        
        Graphics::CommandBuffer& cmd_buf = m_renderer->GetCommandBuffer(scene_view._cmd_buf_idx);
        
        // Frustum cull Renderable array and fill DrawItem array
        
        for (int i = 0; i < m_renderable_count; i++)
        {
            Renderable& renderable = m_renderables[i];
            
            for (int j = 0; j < renderable._mesh->MeshCount; j++)
            {
                int index = scene_view._num_items;
                Graphics::DrawItem& draw_item = scene_view._draw_items[index];
                
                draw_item.uniforms = uniform_allocator->NewPerFrame<Graphics::PerDrawUniforms>();
                draw_item.uniforms->model = renderable._transform->global_transform;
                draw_item.uniforms->position = renderable._transform->position;
                draw_item.base_index = renderable._mesh->SubMeshes[j].m_BaseIndex;
                draw_item.base_vertex = renderable._mesh->SubMeshes[j].m_BaseVertex;
                draw_item.index_count = renderable._mesh->SubMeshes[j].m_IndexCount;
                draw_item.material = renderable._mesh->SubMeshes[j]._material;
                draw_item.vertex_array = renderable._mesh->VertexArray;
                draw_item.type = renderable._type;
                
                scene_view._num_items++;
            }
        }
        
        // Sort DrawItem array
        
        std::partial_sort(scene_view._draw_items.begin(),
                          scene_view._draw_items.begin() + scene_view._num_items,
                          scene_view._draw_items.end(),
                          DrawItemSort);
        
        // Fill CommandBuffer while skipping redundant state changes
        
        for (int i = 0; i < scene_view._rendering_path->_num_render_passes; i++)
        {
            Graphics::RenderPass& render_pass = scene_view._rendering_path->_render_passes[i];
            
            for (auto sub_pass : render_pass.sub_passes)
            {
                // Bind Framebuffer Command
                
                cmd_buf.Write(Graphics::CommandType::BindFramebuffer);
                
                Graphics::BindFramebufferCmdData cmd1;
                cmd1.framebuffer = sub_pass.framebuffer_target;
                
                cmd_buf.Write<Graphics::BindFramebufferCmdData>(&cmd1);
                
                // Clear Framebuffer
                
                cmd_buf.Write(Graphics::CommandType::ClearFramebuffer);
                
                Graphics::ClearFramebufferCmdData cmd2;
                cmd2.clear_target = FramebufferClearTarget::ALL;
                cmd2.clear_color  = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
                
                cmd_buf.Write<Graphics::ClearFramebufferCmdData>(&cmd2);
                
                // Bind Per Frame Globle Uniforms
                
                cmd_buf.Write(Graphics::CommandType::BindUniformBuffer);
                
                Graphics::BindUniformBufferCmdData cmd3;
                cmd3.buffer = m_renderer->m_per_frame_buffer;
                cmd3.slot = PER_FRAME_UNIFORM_SLOT;
                
                cmd_buf.Write<Graphics::BindUniformBufferCmdData>(&cmd3);
                
                // Copy Per Frame data
                
                cmd_buf.Write(Graphics::CommandType::CopyUniformData);
                
                Graphics::CopyUniformCmdData cmd4;
                cmd4.buffer = m_renderer->m_per_frame_buffer;
                cmd4.data   = per_frame;
                cmd4.size   = sizeof(Graphics::PerFrameUniforms);
                cmd4.map_type = BufferMapType::WRITE;
                
                cmd_buf.Write<Graphics::CopyUniformCmdData>(&cmd4);
                
                if(render_pass.geometry_type == GeometryType::SCENE)
                {
                    uint16 last_vao = 0;
                    uint16 last_program = 0;
                    uint16 last_texture = 0;
                    uint16 last_sampler = 0;
                    
                    for (int j = 0; j < scene_view._num_items; j++)
                    {
                        Graphics::DrawItem& draw_item = scene_view._draw_items[j];
                        
                        if(last_vao != draw_item.vertex_array->m_resource_id)
                        {
                            last_vao = draw_item.vertex_array->m_resource_id;
                            
                            // Bind Vertex Array
                            
                            cmd_buf.Write(Graphics::CommandType::BindVertexArray);
                            
                            Graphics::BindVertexArrayCmdData cmd5;
                            cmd5.vertex_array = draw_item.vertex_array;
                            
                            cmd_buf.Write<Graphics::BindVertexArrayCmdData>(&cmd5);
                        }
                        
                        Resource::ShaderKey key;
                        
                        key.EncodeAlbedo(true);
                        key.EncodeMeshType(draw_item.type);
                        key.EncodeNormal(true);
                        key.EncodeMetalness(true);
                        key.EncodeRoughness(true);
                        key.EncodeParallaxOcclusion(false);
                        
                        // Send to rendering thread pool
                        Graphics::ShaderProgram* program = m_shader_cache->Load(key);
                        
                        if(last_program != program->m_resource_id)
                        {
                            last_program = program->m_resource_id;
                            
                            // Bind Shader Program
                            
                            cmd_buf.Write(Graphics::CommandType::BindShaderProgram);
                            
                            Graphics::BindShaderProgramCmdData cmd6;
                            cmd6.program = program;
                            
                            cmd_buf.Write<Graphics::BindShaderProgramCmdData>(&cmd6);
                        }
                        
                        // Bind Per Frame Globle Uniforms
                        
                        cmd_buf.Write(Graphics::CommandType::BindUniformBuffer);
                        
                        Graphics::BindUniformBufferCmdData cmd7;
                        cmd7.buffer = m_renderer->m_per_draw_buffer;
                        cmd7.slot = PER_DRAW_UNIFORM_SLOT;
                        
                        cmd_buf.Write<Graphics::BindUniformBufferCmdData>(&cmd7);

                        
                        // Copy Per Draw Uniforms
                        
                        cmd_buf.Write(Graphics::CommandType::CopyUniformData);
                        
                        Graphics::CopyUniformCmdData cmd8;
                        cmd8.buffer = m_renderer->m_per_draw_buffer;
                        cmd8.data   = draw_item.uniforms;
                        cmd8.size   = sizeof(Graphics::PerDrawUniforms);
                        cmd8.map_type = BufferMapType::WRITE;
                        
                        cmd_buf.Write<Graphics::CopyUniformCmdData>(&cmd8);
                        
                        for (int k = 0; k < 5 ; k++)
                        {
                            if(draw_item.material->texture_maps[k])
                            {
                                if(last_sampler != draw_item.material->sampler->m_resource_id)
                                {
                                    last_sampler = draw_item.material->sampler->m_resource_id;
                                    
                                    // Bind Sampler State
                                    
                                    cmd_buf.Write(Graphics::CommandType::BindSamplerState);
                                    
                                    Graphics::BindSamplerStateCmdData cmd9;
                                    cmd9.state = draw_item.material->sampler;
                                    cmd9.slot = k;
                                    cmd9.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.Write<Graphics::BindSamplerStateCmdData>(&cmd9);
                                }
                                
                                if(last_texture != draw_item.material->texture_maps[k]->m_resource_id)
                                {
                                    last_texture = draw_item.material->texture_maps[k]->m_resource_id;
                                    
                                    // Bind Textures
                                    
                                    cmd_buf.Write(Graphics::CommandType::BindTexture2D);
                                    
                                    Graphics::BindTexture2DCmdData cmd10;
                                    cmd10.texture = draw_item.material->texture_maps[k];
                                    cmd10.slot = k;
                                    cmd10.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.Write<Graphics::BindTexture2DCmdData>(&cmd10);
                                }
                            }
                            
                        }
                        
                        // Draw
                        
                        cmd_buf.Write(Graphics::CommandType::DrawIndexedBaseVertex);
                        
                        Graphics::DrawIndexedBaseVertexCmdData cmd11;
                        cmd11.base_index = draw_item.base_index;
                        cmd11.base_vertex = draw_item.base_vertex;
                        cmd11.index_count = draw_item.index_count;
                        
                        cmd_buf.Write<Graphics::DrawIndexedBaseVertexCmdData>(&cmd11);
                    }
                }
                else if(render_pass.geometry_type == GeometryType::QUAD)
                {
                    
                }
                
                
            }
        }
        
        cmd_buf.WriteEnd();
    }
    
} }

