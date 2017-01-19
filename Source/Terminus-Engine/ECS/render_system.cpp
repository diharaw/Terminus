#include <ECS/render_system.h>
#include <ECS/scene.h>
#include <Resource/shader_cache.h>
#include <ECS/component_types.h>

namespace terminus
{    
    // Sort Method
    
    bool DrawItemSort(DrawItem i, DrawItem j)
    {
        return i.sort_key.key > j.sort_key.key;
    }

    RenderSystem::RenderSystem()
    {
        _view_count = 0;
        _thread_pool = Global::GetDefaultThreadPool();
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    void RenderSystem::SetRenderDevice(Renderer* renderer)
    {
        _renderer = renderer;
    }
    
    void RenderSystem::SetShaderCache(ShaderCache* shaderCache)
    {
        _shader_cache = shaderCache;
    }
    
    void RenderSystem::Initialize()
    {
		CameraComponent* camera_array = _scene->_camera_pool.get_array();
		int num_cameras = _scene->_camera_pool.get_num_objects();
        
        // For each Scene Camera
        for (int i = 0; i < num_cameras; i++)
        {
            SceneView& view = _views[_view_count++];
            view._screen_rect = camera_array[i].screen_rect;
            view._is_shadow = false;
            view._cmd_buf_idx = _renderer->create_command_buffer();
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (int i = 0; i < _scene->_entities._num_objects; i++)
        {
			Entity& entity = _scene->_entities._objects[i];

            if(_scene->has_transform_component(entity) && _scene->has_mesh_component(entity))
            {
                MeshComponent& mesh_component = _scene->get_mesh_component(entity);
                TransformComponent& transform_component = _scene->get_transform_component(entity);
                
                Renderable& renderable = _renderables[_renderable_count++];
                
                renderable._mesh = mesh_component.mesh;
                renderable._sub_mesh_cull = mesh_component.cull_submeshes;
                // TODO : assign Radius. Add to MeshImporter.
                renderable._transform = &transform_component._global_transform;
				renderable._position = &transform_component._position;
                
                if(mesh_component.mesh->IsSkeletal)
                    renderable._type = RenderableType::SkeletalMesh;
                else if(mesh_component.mesh->IsSkeletal)
                    renderable._type = RenderableType::StaticMesh;
                
                
                // TODO: Create task for generating Shader permutations per view, per renderables
                for (int i = 0; i < renderable._mesh->MeshCount; i++)
                {
                    ShaderKey key;
                    
                    key.EncodeAlbedo(true);
                    key.EncodeMeshType(renderable._type);
                    key.EncodeNormal(true);
                    key.EncodeMetalness(true);
                    key.EncodeRoughness(true);
                    key.EncodeParallaxOcclusion(false);
                    
                    // Send to rendering thread pool
					_shader_cache->load(key);
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
        _renderer->uniform_allocator()->Clear();
        
        int worker_count = _thread_pool->get_num_worker_threads();
        
        // Assign views to threads and frustum cull, sort and fill command buffers in parallel.
        
        int items_per_thread = std::floor((float)_view_count / (float)worker_count);
        
        if(items_per_thread == 0)
            items_per_thread = _view_count;
        
        int submitted_items = 0;
        int scene_index = 0;
        bool is_done = false;
        
        while(!is_done)
        {
            int remaining_items = _view_count - submitted_items;
            
            if(remaining_items <= items_per_thread)
            {
                is_done = true;
                items_per_thread = remaining_items;
            }
            
            Task task;
            RenderPrepareTaskData* data = task_data<RenderPrepareTaskData>(task);
    
            data->_scene_index = scene_index++;
            
            task._function.Bind<RenderSystem, &RenderSystem::RenderPrepareTask>(this);
            
            submitted_items += items_per_thread;
            
            _thread_pool->enqueue(task);
        }
        
        _thread_pool->wait();
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
        SceneView& scene_view = _views[_data->_scene_index];
     
        LinearAllocator* uniform_allocator = _renderer->uniform_allocator();
        uniform_allocator->Clear();
        
        // Set up Per frame uniforms
        
        PerFrameUniforms* per_frame = uniform_allocator->NewPerFrame<PerFrameUniforms>();
        per_frame->projection = *scene_view._projection_matrix;
        per_frame->view       = *scene_view._view_matrix;
        
        CommandBuffer& cmd_buf = _renderer->command_buffer(scene_view._cmd_buf_idx);
        
        // Frustum cull Renderable array and fill DrawItem array
        
        for (int i = 0; i < _renderable_count; i++)
        {
            Renderable& renderable = _renderables[i];
            
            for (int j = 0; j < renderable._mesh->MeshCount; j++)
            {
                int index = scene_view._num_items;
                DrawItem& draw_item = scene_view._draw_items[index];
                
                draw_item.uniforms = uniform_allocator->NewPerFrame<PerDrawUniforms>();
                draw_item.uniforms->model = *renderable._transform;
                draw_item.uniforms->position = *renderable._position;
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
            RenderPass& render_pass = scene_view._rendering_path->_render_passes[i];
            
            for (auto sub_pass : render_pass.sub_passes)
            {
                // Bind Framebuffer Command
                
                cmd_buf.Write(CommandType::BindFramebuffer);
                
                BindFramebufferCmdData cmd1;
                cmd1.framebuffer = sub_pass.framebuffer_target;
                
                cmd_buf.Write<BindFramebufferCmdData>(&cmd1);
                
                // Clear Framebuffer
                
                cmd_buf.Write(CommandType::ClearFramebuffer);
                
                ClearFramebufferCmdData cmd2;
                cmd2.clear_target = FramebufferClearTarget::ALL;
                cmd2.clear_color  = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
                
                cmd_buf.Write<ClearFramebufferCmdData>(&cmd2);
                
                // Bind Per Frame Globle Uniforms
                
                cmd_buf.Write(CommandType::BindUniformBuffer);
                
                BindUniformBufferCmdData cmd3;
                cmd3.buffer = _renderer->_per_frame_buffer;
                cmd3.slot = PER_FRAME_UNIFORM_SLOT;
                
                cmd_buf.Write<BindUniformBufferCmdData>(&cmd3);
                
                // Copy Per Frame data
                
                cmd_buf.Write(CommandType::CopyUniformData);
                
                CopyUniformCmdData cmd4;
                cmd4.buffer = _renderer->_per_frame_buffer;
                cmd4.data   = per_frame;
                cmd4.size   = sizeof(PerFrameUniforms);
                cmd4.map_type = BufferMapType::WRITE;
                
                cmd_buf.Write<CopyUniformCmdData>(&cmd4);
                
                if(render_pass.geometry_type == GeometryType::SCENE)
                {
                    uint16 last_vao = 0;
                    uint16 last_program = 0;
                    uint16 last_texture = 0;
                    uint16 last_sampler = 0;
                    
                    for (int j = 0; j < scene_view._num_items; j++)
                    {
                        DrawItem& draw_item = scene_view._draw_items[j];
                        
                        if(last_vao != draw_item.vertex_array->m_resource_id)
                        {
                            last_vao = draw_item.vertex_array->m_resource_id;
                            
                            // Bind Vertex Array
                            
                            cmd_buf.Write(CommandType::BindVertexArray);
                            
                            BindVertexArrayCmdData cmd5;
                            cmd5.vertex_array = draw_item.vertex_array;
                            
                            cmd_buf.Write<BindVertexArrayCmdData>(&cmd5);
                        }
                        
                        ShaderKey key;
                        
                        key.EncodeAlbedo(true);
                        key.EncodeMeshType(draw_item.type);
                        key.EncodeNormal(true);
                        key.EncodeMetalness(true);
                        key.EncodeRoughness(true);
                        key.EncodeParallaxOcclusion(false);
                        
                        // Send to rendering thread pool
                        ShaderProgram* program = _shader_cache->load(key);
                        
                        if(last_program != program->m_resource_id)
                        {
                            last_program = program->m_resource_id;
                            
                            // Bind Shader Program
                            
                            cmd_buf.Write(CommandType::BindShaderProgram);
                            
                            BindShaderProgramCmdData cmd6;
                            cmd6.program = program;
                            
                            cmd_buf.Write<BindShaderProgramCmdData>(&cmd6);
                        }
                        
                        // Bind Per Frame Globle Uniforms
                        
                        cmd_buf.Write(CommandType::BindUniformBuffer);
                        
                        BindUniformBufferCmdData cmd7;
                        cmd7.buffer = _renderer->_per_draw_buffer;
                        cmd7.slot = PER_DRAW_UNIFORM_SLOT;
                        
                        cmd_buf.Write<BindUniformBufferCmdData>(&cmd7);

                        
                        // Copy Per Draw Uniforms
                        
                        cmd_buf.Write(CommandType::CopyUniformData);
                        
                        CopyUniformCmdData cmd8;
                        cmd8.buffer = _renderer->_per_draw_buffer;
                        cmd8.data   = draw_item.uniforms;
                        cmd8.size   = sizeof(PerDrawUniforms);
                        cmd8.map_type = BufferMapType::WRITE;
                        
                        cmd_buf.Write<CopyUniformCmdData>(&cmd8);
                        
                        for (int k = 0; k < 5 ; k++)
                        {
                            if(draw_item.material->texture_maps[k])
                            {
                                if(last_sampler != draw_item.material->sampler->m_resource_id)
                                {
                                    last_sampler = draw_item.material->sampler->m_resource_id;
                                    
                                    // Bind Sampler State
                                    
                                    cmd_buf.Write(CommandType::BindSamplerState);
                                    
                                    BindSamplerStateCmdData cmd9;
                                    cmd9.state = draw_item.material->sampler;
                                    cmd9.slot = k;
                                    cmd9.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.Write<BindSamplerStateCmdData>(&cmd9);
                                }
                                
                                if(last_texture != draw_item.material->texture_maps[k]->m_resource_id)
                                {
                                    last_texture = draw_item.material->texture_maps[k]->m_resource_id;
                                    
                                    // Bind Textures
                                    
                                    cmd_buf.Write(CommandType::BindTexture2D);
                                    
                                    BindTexture2DCmdData cmd10;
                                    cmd10.texture = draw_item.material->texture_maps[k];
                                    cmd10.slot = k;
                                    cmd10.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.Write<BindTexture2DCmdData>(&cmd10);
                                }
                            }
                            
                        }
                        
                        // Draw
                        
                        cmd_buf.Write(CommandType::DrawIndexedBaseVertex);
                        
                        DrawIndexedBaseVertexCmdData cmd11;
                        cmd11.base_index = draw_item.base_index;
                        cmd11.base_vertex = draw_item.base_vertex;
                        cmd11.index_count = draw_item.index_count;
                        
                        cmd_buf.Write<DrawIndexedBaseVertexCmdData>(&cmd11);
                    }
                }
                else if(render_pass.geometry_type == GeometryType::QUAD)
                {
                    
                }
                
                
            }
        }
        
        cmd_buf.WriteEnd();
    }
} // namespace terminus

