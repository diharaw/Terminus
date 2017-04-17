#include <gameplay/render_system.h>
#include <gameplay/scene.h>
#include <gameplay/component_types.h>
#include <resource/shader_cache.h>
#include <core/context.h>
#include <utility/profiler.h>

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
        _thread_pool = global::get_default_threadpool();
        _renderer = &context::get_renderer();
        _shader_cache = &context::get_shader_cache();
        
        for (ID& entity_id : _entity_renderable_ref)
        {
            entity_id = INVALID_ID;
        }
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    void RenderSystem::initialize(Scene* scene)
    {
        _scene = scene;
        
		CameraComponent* camera_array = _scene->_camera_pool.get_array();
		int num_cameras = _scene->_camera_pool.get_num_objects();
        
        _skydome_mesh = context::get_mesh_cache().load("sky_dome.tsm");
        
        if(scene->_sky_pool.get_num_objects() > 0)
            _sky_cmp = scene->_sky_pool.get_array();
        else
            _sky_cmp = nullptr;
        
        // For each Scene Camera
        for (int i = 0; i < num_cameras; i++)
        {
            SceneView& view = _views[_view_count++];
            view._screen_rect = camera_array[i].screen_rect;
            view._is_shadow = false;
            view._cmd_buf_idx = _renderer->create_command_buffer();
            view._rendering_path = camera_array[i].rendering_path;
            //view._projection_matrix = &camera_array[i].camera.m_ProjectionMatrix;
            //view._view_matrix = &camera_array[i].camera.m_ViewMatrix;
            //view._view_projection_matrix = &camera_array[i].camera.m_ViewProjection;
            
            view._projection_matrix = &camera_array[i].projection_matrix;
            view._view_matrix = &camera_array[i].view_matrix;
            view._view_projection_matrix = &camera_array[i].view_projection_matrix;
            
            view._num_items = 0;
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (int i = 0; i < _scene->_entities._num_objects; i++)
        {
			Entity& entity = _scene->_entities._objects[i];
            on_entity_created(entity);
        }
        
        _skydome_item.base_index = _skydome_mesh->SubMeshes[0].m_BaseIndex;
        _skydome_item.base_vertex = _skydome_mesh->SubMeshes[0].m_BaseVertex;
        _skydome_item.index_count = _skydome_mesh->SubMeshes[0].m_IndexCount;
        _skydome_item.material = _skydome_mesh->SubMeshes[0]._material;
        _skydome_item.vertex_array = _skydome_mesh->VertexArray;
        _skydome_item.type = RenderableType::Skybox;
        
        for(int i = 0; i < _view_count; i++)
        {
            RenderingPath* path = _views[i]._rendering_path;
            
            for (int j = 0; j < path->_num_render_passes; j++)
            {
                RenderPass* render_pass = path->_render_passes[j];
                
                if(render_pass->geometry_type == GeometryType::SKY)
                {
                    for(RenderSubPass& sub_pass : render_pass->sub_passes)
                        _shader_cache->load(RenderableType::Skybox, render_pass->pass_id, &sub_pass, nullptr);
                }
            }
        }
    }
    
    void RenderSystem::update(double delta)
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
    
    void RenderSystem::shutdown()
    {
        for (int i = 0; i < _renderables._num_objects; i++)
        {
            // unload mesh
            MeshCache& cache = context::get_mesh_cache();
            cache.unload(_renderables._objects[i]._mesh);
        }
    }
    
    void RenderSystem::on_entity_created(Entity entity)
    {
        if(_scene->has_transform_component(entity) && _scene->has_mesh_component(entity))
        {
            MeshComponent& mesh_component = _scene->get_mesh_component(entity);
            TransformComponent& transform_component = _scene->get_transform_component(entity);
            
            ID renderable_id = _renderables.add();
            
            _entity_renderable_ref[INDEX_FROM_ID(entity._id)] = renderable_id;
            Renderable& renderable = _renderables.lookup(renderable_id);
            
            renderable._mesh = mesh_component.mesh;
            renderable._sub_mesh_cull = mesh_component.cull_submeshes;
            // TODO : assign Radius. Add to MeshImporter.
            renderable._transform = &transform_component._global_transform;
            renderable._position = &transform_component._position;
            
            if(mesh_component.mesh->IsSkeletal)
                renderable._type = RenderableType::SkeletalMesh;
            else if(mesh_component.mesh->IsSkeletal)
                renderable._type = RenderableType::StaticMesh;
            
            for(int i = 0; i < _view_count; i++)
            {
                RenderingPath* path = _views[i]._rendering_path;
                
                for (int j = 0; j < path->_num_render_passes; j++)
                {
                    RenderPass* render_pass = path->_render_passes[j];
                    
                    if(render_pass->geometry_type == GeometryType::SCENE)
                    {
                        for(RenderSubPass& sub_pass : render_pass->sub_passes)
                        {
                            for (int x = 0; x < renderable._mesh->MeshCount; x++)
                            {
                                Material* mat = renderable._mesh->SubMeshes[x]._material;
                                _shader_cache->load(renderable._type, render_pass->pass_id, &sub_pass, mat);
                            }
                        }
                    }
                }
            }
        }
    }
    
    void RenderSystem::on_entity_destroyed(Entity entity)
    {
        if(_entity_renderable_ref[entity._id] != INVALID_ID)
        {
            ID renderable_id = _entity_renderable_ref[entity._id];
            
            // TODO: should i clean up resources here?
            
            _renderables.remove(renderable_id);
            _entity_renderable_ref[entity._id] = INVALID_ID;
        }
    }
    
    TASK_METHOD_DEFINITION(RenderSystem, RenderPrepareTask)
    {
		TERMINUS_BEGIN_CPU_PROFILE(frustum_cull);

        RenderPrepareTaskData* _data = static_cast<RenderPrepareTaskData*>(data);
        SceneView& scene_view = _views[_data->_scene_index];
     
        LinearAllocator* uniform_allocator = _renderer->uniform_allocator();
        uniform_allocator->Clear();
        
        // Set up Per frame uniforms
        
        PerFrameUniforms* per_frame = uniform_allocator->NewPerFrame<PerFrameUniforms>();
        per_frame->projection = *scene_view._projection_matrix;
        per_frame->view       = *scene_view._view_matrix;
        
        PerFrameSkyUniforms* per_frame_sky = uniform_allocator->NewPerFrame<PerFrameSkyUniforms>();
        per_frame_sky->projection = *scene_view._projection_matrix;
        per_frame_sky->view       = Matrix4(Matrix3(*scene_view._view_matrix));
       
        CommandBuffer& cmd_buf = _renderer->command_buffer(scene_view._cmd_buf_idx);
        
        // Frustum cull Renderable array and fill DrawItem array
        
        scene_view._num_items = 0;
        
        for (int i = 0; i < _renderables._num_objects; i++)
        {
            Renderable& renderable = _renderables._objects[i];
         
            if(!renderable._mesh->VertexArray)
            {
                T_LOG_ERROR("NULL Vertex Array");
            }
            else
            {
                for (int j = 0; j < renderable._mesh->MeshCount; j++)
                {
                    int index = scene_view._num_items;
                    DrawItem& draw_item = scene_view._draw_items[index];
                    
                    draw_item.uniforms = uniform_allocator->NewPerFrame<PerDrawUniforms>();
                    draw_item.uniforms->model = *renderable._transform;
                    draw_item.uniforms->position = *renderable._position;
                    draw_item.uniforms->model_view_projection = *scene_view._view_projection_matrix * draw_item.uniforms->model;
                    draw_item.base_index = renderable._mesh->SubMeshes[j].m_BaseIndex;
                    draw_item.base_vertex = renderable._mesh->SubMeshes[j].m_BaseVertex;
                    draw_item.index_count = renderable._mesh->SubMeshes[j].m_IndexCount;
                    draw_item.material = renderable._mesh->SubMeshes[j]._material;
                    draw_item.vertex_array = renderable._mesh->VertexArray;
                    draw_item.type = renderable._type;
                    draw_item.renderable_id = i;
                    
                    scene_view._num_items++;
                }
            }
        }
        
		TERMINUS_END_CPU_PROFILE;

		// Sort DrawItem array

		TERMINUS_BEGIN_CPU_PROFILE(sort);

        std::sort(scene_view._draw_items.begin(),
                  scene_view._draw_items.begin() + scene_view._num_items,
                  DrawItemSort);

        TERMINUS_END_CPU_PROFILE;
        
        // Fill CommandBuffer while skipping redundant state changes

		TERMINUS_BEGIN_CPU_PROFILE(command_generation);
        
        bool cleared_default = false;
        
        for (int i = 0; i < scene_view._rendering_path->_num_render_passes; i++)
        {
            RenderPass* render_pass = scene_view._rendering_path->_render_passes[i];
            
            for (int j = 0; j < render_pass->num_sub_passes; j++)
            {
                RenderSubPass& sub_pass = render_pass->sub_passes[j];
                
                // Bind Pipeline State Object
                
                cmd_buf.Write(CommandType::BindPipelineStateObject);
                
                BindPipelineStateObjectData cmd0;
                cmd0.pso = sub_pass.pso;
                
                cmd_buf.Write<BindPipelineStateObjectData>(&cmd0);
                
                // Bind Framebuffer Command
                
                cmd_buf.Write(CommandType::BindFramebuffer);
                
                BindFramebufferCmdData cmd1;
                cmd1.framebuffer = sub_pass.framebuffer_target;
                
                cmd_buf.Write<BindFramebufferCmdData>(&cmd1);
                
                // Clear Framebuffer
                
                if(sub_pass.framebuffer_target == nullptr && !cleared_default)
                {
                    cleared_default = true;
                    
                    cmd_buf.Write(CommandType::ClearFramebuffer);
                    
                    ClearFramebufferCmdData cmd2;
                    cmd2.clear_target = FramebufferClearTarget::ALL;
                    cmd2.clear_color  = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
                    
                    cmd_buf.Write<ClearFramebufferCmdData>(&cmd2);
                }
                
                // Copy Per Frame data
                
                cmd_buf.Write(CommandType::CopyUniformData);
                
                CopyUniformCmdData cmd4;
                cmd4.buffer = _renderer->_per_frame_buffer;
                cmd4.data   = per_frame;
                cmd4.size   = sizeof(PerFrameUniforms);
                cmd4.map_type = BufferMapType::WRITE;
                
                cmd_buf.Write<CopyUniformCmdData>(&cmd4);
                
                // Bind Per Frame Global Uniforms
                
                cmd_buf.Write(CommandType::BindUniformBuffer);
                
                BindUniformBufferCmdData cmd3;
                cmd3.buffer = _renderer->_per_frame_buffer;
                cmd3.shader_type = ShaderType::VERTEX;
                cmd3.slot = PER_FRAME_UNIFORM_SLOT;
                
                cmd_buf.Write<BindUniformBufferCmdData>(&cmd3);
                
                if(render_pass->geometry_type == GeometryType::SCENE)
                {
                    int16 last_vao = -1;
                    int16 last_program = -1;
                    int16 last_texture = -1;
                    int   last_renderable = -1;
                    
                    for (int k = 0; k < scene_view._num_items; k++)
                    {
                        DrawItem& draw_item = scene_view._draw_items[k];
                        
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
                        
                        key.encode_mesh_type(draw_item.type);
                        key.encode_renderpass_id(render_pass->pass_id);
                        
                        if(draw_item.material)
                        {
                            key.encode_albedo(draw_item.material->texture_maps[DIFFUSE]);
                            key.encode_normal(draw_item.material->texture_maps[NORMAL]);
                            key.encode_metalness(draw_item.material->texture_maps[METALNESS]);
                            key.encode_roughness(draw_item.material->texture_maps[ROUGHNESS]);
                            key.encode_parallax_occlusion(draw_item.material->texture_maps[DISPLACEMENT]);
                        }
                        
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
                        
                        if(last_renderable != draw_item.renderable_id)
                        {
                            last_renderable = draw_item.renderable_id;
                            
                            // Copy Per Draw Uniforms
                            
                            cmd_buf.Write(CommandType::CopyUniformData);
                            
                            CopyUniformCmdData cmd8;
                            cmd8.buffer = _renderer->_per_draw_buffer;
                            cmd8.data   = draw_item.uniforms;
                            cmd8.size   = sizeof(PerDrawUniforms);
                            cmd8.map_type = BufferMapType::WRITE;
                            
                            cmd_buf.Write<CopyUniformCmdData>(&cmd8);
                            
                            // Bind Per Draw Uniforms
                            
                            cmd_buf.Write(CommandType::BindUniformBuffer);
                            
                            BindUniformBufferCmdData cmd7;
                            cmd7.buffer = _renderer->_per_draw_buffer;
                            cmd7.shader_type = ShaderType::VERTEX;
                            cmd7.slot = PER_DRAW_UNIFORM_SLOT;
                            
                            cmd_buf.Write<BindUniformBufferCmdData>(&cmd7);
                        }
                        
                        for (int l = 0; l < 5 ; l++)
                        {
                            if(draw_item.material)
                            {
                                if(draw_item.material->texture_maps[l])
                                {
                                    // Bind Sampler State
                                    
                                    cmd_buf.Write(CommandType::BindSamplerState);
                                    
                                    BindSamplerStateCmdData cmd9;
                                    cmd9.state = draw_item.material->sampler;
                                    cmd9.slot = l;
                                    cmd9.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.Write<BindSamplerStateCmdData>(&cmd9);
                                    
                                    if(last_texture != draw_item.material->texture_maps[l]->m_resource_id)
                                    {
                                        last_texture = draw_item.material->texture_maps[l]->m_resource_id;
                                        
                                        // Bind Textures
                                        
                                        cmd_buf.Write(CommandType::BindTexture);
                                        
                                        BindTextureCmdData cmd10;
                                        cmd10.texture = draw_item.material->texture_maps[l];
                                        cmd10.slot = l;
                                        cmd10.shader_type = ShaderType::PIXEL;
                                        
                                        cmd_buf.Write<BindTextureCmdData>(&cmd10);
                                    }
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
                else if(render_pass->geometry_type == GeometryType::SKY)
                {
                    // Bind Vertex Array
                    
                    cmd_buf.Write(CommandType::BindVertexArray);
                    
                    BindVertexArrayCmdData cmd5;
                    cmd5.vertex_array = _skydome_item.vertex_array;
                    
                    cmd_buf.Write<BindVertexArrayCmdData>(&cmd5);
                    
                    ShaderKey key;
                    
                    key.encode_mesh_type(_skydome_item.type);
                    key.encode_renderpass_id(render_pass->pass_id);
                    
                    // Send to rendering thread pool
                    ShaderProgram* program = _shader_cache->load(key);
                    
                    // Bind Shader Program
                    
                    cmd_buf.Write(CommandType::BindShaderProgram);
                    
                    BindShaderProgramCmdData cmd6;
                    cmd6.program = program;
                    
                    cmd_buf.Write<BindShaderProgramCmdData>(&cmd6);
                    
                    // Copy Per Frame data
                    
                    cmd_buf.Write(CommandType::CopyUniformData);
                    
                    CopyUniformCmdData cmd4;
                    cmd4.buffer = _renderer->_per_frame_sky_buffer;
                    cmd4.data   = per_frame_sky;
                    cmd4.size   = sizeof(PerFrameSkyUniforms);
                    cmd4.map_type = BufferMapType::WRITE;
                    
                    cmd_buf.Write<CopyUniformCmdData>(&cmd4);
                    
                    // Bind Per Frame Sky Global Uniforms
                    
                    cmd_buf.Write(CommandType::BindUniformBuffer);
                    
                    BindUniformBufferCmdData cmd3;
                    cmd3.buffer = _renderer->_per_frame_sky_buffer;
                    cmd3.shader_type = ShaderType::VERTEX;
                    cmd3.slot = PER_FRAME_UNIFORM_SLOT;
                    
                    cmd_buf.Write<BindUniformBufferCmdData>(&cmd3);
                    
                    // Bind Sampler State
                    
                    cmd_buf.Write(CommandType::BindSamplerState);
                    
                    BindSamplerStateCmdData cmd9;
                    cmd9.state = _sky_cmp->sampler;
                    cmd9.slot = 0;
                    cmd9.shader_type = ShaderType::PIXEL;
                    
                    cmd_buf.Write<BindSamplerStateCmdData>(&cmd9);
                    
                    // Bind Cubemap
                    
                    cmd_buf.Write(CommandType::BindTexture);
                    
                    BindTextureCmdData cmd10;
                    cmd10.texture = _sky_cmp->texture;
                    cmd10.slot = 0;
                    cmd10.shader_type = ShaderType::PIXEL;
                    
                    cmd_buf.Write<BindTextureCmdData>(&cmd10);
                    
                    // Draw
                    
                    cmd_buf.Write(CommandType::DrawIndexedBaseVertex);
                    
                    DrawIndexedBaseVertexCmdData cmd11;
                    cmd11.base_index = _skydome_item.base_index;
                    cmd11.base_vertex = _skydome_item.base_vertex;
                    cmd11.index_count = _skydome_item.index_count;
                    
                    cmd_buf.Write<DrawIndexedBaseVertexCmdData>(&cmd11);
                }
                else if(render_pass->geometry_type == GeometryType::QUAD)
                {
                    
                }
            }
        }
        
        cmd_buf.WriteEnd();
        
        // reset
        scene_view._num_items = 0;

		TERMINUS_END_CPU_PROFILE;
    }
} // namespace terminus

