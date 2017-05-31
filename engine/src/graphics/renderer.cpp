#include <graphics/renderer.h>
#include <graphics/imgui_backend.h>
#include <core/config.h>
#include <core/context.h>
#include <gameplay/scene.h>
#include <resource/shader_cache.h>
#include <core/frame_packet.h>
#include <utility/profiler.h>
#include <graphics/command_buffer.h>
#include <core/sync.h>

TERMINUS_BEGIN_NAMESPACE

// Sort Method

bool DrawItemSort(DrawItem i, DrawItem j)
{
    return i.sort_key.key > j.sort_key.key;
}

bool scene_view_z_index_sort(SceneView i, SceneView j)
{
	return i.z_index > j.z_index;
}

Renderer::Renderer()
{
    m_thread_pool = global::get_default_threadpool();
}

Renderer::~Renderer()
{
    
}

void Renderer::initialize(FramePacket* pkts)
{
	m_pkt = pkts;
	m_thread = std::thread(&Renderer::render_loop, this);
	sync::notify_main_ready();
	sync::wait_for_renderer_ready();
}

void Renderer::initialize_internal()
{
	RenderDevice& device = context::get_render_device();

	m_graphics_queue = device.create_command_queue();

	// Initialize the CommandPools in each FramePacket.

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				m_pkt[i].scene_render_states[j].cmd_pool[k] = device.create_command_pool();
				m_pkt[i].scene_render_states[j].cmd_buffers[k] = device.create_command_buffer(m_pkt[i].scene_render_states[j].cmd_pool[k]);
			}
		}
	}

	BufferCreateDesc desc;

	desc.data = nullptr;
	desc.usage_type = BufferUsageType::DYNAMIC;
	desc.size = sizeof(PerFrameUniforms);

	m_per_frame_buffer = device.create_uniform_buffer(desc);

	desc.size = sizeof(PerFrameSkyUniforms);

	m_per_frame_sky_buffer = device.create_uniform_buffer(desc);

	desc.size = sizeof(PerDrawUniforms);

	m_per_draw_buffer = device.create_uniform_buffer(desc);

	desc.size = sizeof(PerDrawMaterialUniforms);

	m_per_draw_material_buffer = device.create_uniform_buffer(desc);

	desc.size = sizeof(PerDrawBoneOffsetUniforms);

	m_per_draw_bone_offsets_buffer = device.create_uniform_buffer(desc);
}

void Renderer::shutdown_internal()
{
	RenderDevice& device = context::get_render_device();

	device.destroy_uniform_buffer(m_per_frame_buffer);
	device.destroy_uniform_buffer(m_per_frame_sky_buffer);
	device.destroy_uniform_buffer(m_per_draw_buffer);
	device.destroy_uniform_buffer(m_per_draw_material_buffer);
	device.destroy_uniform_buffer(m_per_draw_bone_offsets_buffer);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				device.destroy_command_buffer(m_pkt[i].scene_render_states[j].cmd_pool[k], m_pkt[i].scene_render_states[j].cmd_buffers[k]);
				device.destroy_command_pool(m_pkt[i].scene_render_states[j].cmd_pool[k]);
			}
		}
	}

	device.destroy_command_queue(m_graphics_queue);
}

void Renderer::shutdown(FramePacket* pkts)
{
	m_pkt = pkts;
	sync::notify_renderer_begin();
}

void Renderer::enqueue_upload_task(Task& task)
{
	concurrent_queue::push(m_graphics_upload_queue, task);
}

void Renderer::generate_commands(Scene* scene)
{
    uint16_t view_count = scene->m_render_system.view_count();
    
    int worker_count = m_thread_pool->get_num_worker_threads();
    
    // Assign views to threads and frustum cull, sort and fill command buffers in parallel.
    
    int items_per_thread = std::floor((float)view_count / (float)worker_count);
    
    if(items_per_thread == 0)
        items_per_thread = view_count;
    
    int submitted_items = 0;
    int scene_index = 0;
    bool is_done = false;
    
    while(!is_done)
    {
        int remaining_items = view_count - submitted_items;
        
        if(remaining_items <= items_per_thread)
        {
            is_done = true;
            items_per_thread = remaining_items;
        }
        
        Task task;
        RenderPrepareTaskData* data = task_data<RenderPrepareTaskData>(task);
        
        data->_scene_index = scene_index++;
        data->_scene = scene;
        
        task._function.Bind<Renderer, &Renderer::generate_commands_view>(this);
        
        submitted_items += items_per_thread;
        
        _thread_pool->enqueue(task);
    }
    
    _thread_pool->wait();

}

void Renderer::render_loop()
{
	Context& context = global::get_context();

	sync::wait_for_main_ready();

	context._render_device.initialize();
	initialize_internal();

	ImGuiBackend* gui_backend = context::get_imgui_backend();

	gui_backend->initialize();
	gui_backend->new_frame();

	sync::notify_renderer_ready();

	while (true)
	{
		sync::wait_for_renderer_begin();

		if (context._shutdown)
			break;

		TERMINUS_BEGIN_CPU_PROFILE(renderer);

		// submit api calls
		context._render_device.submit_command_queue(m_graphics_queue, m_pkt->cmd_buffers[0], m_pkt->cmd_buffer_count);

		// optional waiting in Vulkan/Direct3D 12 API's.

		// do resource uploading. one task per frame for now.
		if (!concurrent_queue::empty(m_graphics_upload_queue))
		{
			Task upload_task = concurrent_queue::pop(m_graphics_upload_queue);
			upload_task._function.Invoke(&upload_task._data[0]);
			sync::notify_loader_wakeup();
		}

		TERMINUS_END_CPU_PROFILE;

		// notify done
		sync::notify_renderer_done();

		gui_backend->render();
		context._render_device.swap_buffers();
	}

	shutdown_internal();

	sync::notify_renderer_exit();
}

void Renderer::submit(FramePacket* pkt)
{
	if (pkt)
	{
		m_pkt = pkt;
		sync::notify_renderer_begin();
	}
	// Wait for submission end
}

void Renderer::render(FramePacket* pkt)
{
	if (pkt)
	{
		// Sort SceneViews according to Z-Index.
		std::sort(std::begin(pkt->views), std::begin(pkt->views) + pkt->total_views, scene_view_z_index_sort);

		for (uint32_t i = 0; i < pkt->total_views; i++)
		{
			SceneView& view = pkt->views[i];

			for (uint32_t j = 0; j < view.rendering_path->_num_render_passes; j++)
			{
				RenderPass* render_pass = view.rendering_path->_render_passes[j];

				switch (render_pass->render_pass_type)
				{
				case RenderPassType::SHADOW_MAP:
				{
					break;
				}
				case RenderPassType::SCENE:
				{
					m_scene_renderer.render(render_pass, pkt);
					break;
				}
				case RenderPassType::SKY:
				{
					break;
				}
				case RenderPassType::UI:
				{
					break;
				}
				case RenderPassType::DEBUG:
				{
					break;
				}
				case RenderPassType::POST_PROCESS:
				{
					break;
				}
				case RenderPassType::COMPOSITION:
				{
					break;
				}
				}

				for (uint32_t k = 0; k < render_pass->num_sub_passes; k++)
				{
					RenderSubPass& sub_pass = render_pass->sub_passes[k];

					
				}
			}
		}
	}
}

void Renderer::generate_commands_view(void* data)
{
    TERMINUS_BEGIN_CPU_PROFILE(frustum_cull);
    
    RenderPrepareTaskData* _data = static_cast<RenderPrepareTaskData*>(data);
    
    RenderSystem& render_system = _data->_scene->_render_system;
    ShaderCache& shader_cache = context::get_shader_cache();
    
    SceneView* view_array = render_system.scene_views();
    SceneView& scene_view = view_array[_data->_scene_index];
    
    LinearAllocator* uniform_allocator = this->uniform_allocator();
    uniform_allocator->Clear();
    
    // Set up Per frame uniforms
    
    PerFrameUniforms* per_frame = uniform_allocator->NewPerFrame<PerFrameUniforms>();
    per_frame->projection = *scene_view._projection_matrix;
    per_frame->view       = *scene_view._view_matrix;
    
    PerFrameSkyUniforms* per_frame_sky = uniform_allocator->NewPerFrame<PerFrameSkyUniforms>();
    per_frame_sky->projection = *scene_view._projection_matrix;
    per_frame_sky->view       = Matrix4(Matrix3(*scene_view._view_matrix));
    
    CommandBuffer& cmd_buf = this->command_buffer(scene_view._cmd_buf_idx);
    
    // Frustum cull Renderable array and fill DrawItem array
    
    scene_view._num_items = 0;
    uint32_t num_renderables = render_system.renderable_count();
    Renderable* renderable_array = render_system.renderables();
    
    for (int i = 0; i < num_renderables; i++)
    {
        Renderable& renderable = renderable_array[i];
        
        if(!renderable._mesh->vertex_array)
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
	uint32_t command_type;

	cmd_buf.start();
    
    for (int i = 0; i < scene_view._rendering_path->_num_render_passes; i++)
    {
        RenderPass* render_pass = scene_view._rendering_path->_render_passes[i];
        
        for (int j = 0; j < render_pass->num_sub_passes; j++)
        {
            RenderSubPass& sub_pass = render_pass->sub_passes[j];
            
            // Bind Pipeline State Object
			command_type = CommandBuffer::BindPipelineStateObject;
            
            cmd_buf.write(command_type);
            
            BindPipelineStateObjectData cmd0;
            cmd0.pso = sub_pass.pso;
            
            cmd_buf.write<BindPipelineStateObjectData>(cmd0);
            
            // Bind Framebuffer Command
            
			command_type = CommandBuffer::BindFramebuffer;
            cmd_buf.write(command_type);
            
            BindFramebufferCmdData cmd1;
            cmd1.framebuffer = sub_pass.framebuffer_target;
            
            cmd_buf.write<BindFramebufferCmdData>(cmd1);
            
            // Clear Framebuffer
            
            if(sub_pass.framebuffer_target == nullptr && !cleared_default)
            {
                cleared_default = true;
                
				command_type = CommandBuffer::ClearFramebuffer;
                cmd_buf.write(command_type);
                
                ClearFramebufferCmdData cmd2;
                cmd2.clear_target = FramebufferClearTarget::ALL;
                cmd2.clear_color  = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
                
                cmd_buf.write<ClearFramebufferCmdData>(cmd2);
            }
            
            // Copy Per Frame data

			command_type = CommandBuffer::CopyUniformData;
            cmd_buf.write(command_type);
            
            CopyUniformCmdData cmd4;
            cmd4.buffer = _per_frame_buffer;
            cmd4.data   = per_frame;
            cmd4.size   = sizeof(PerFrameUniforms);
            cmd4.map_type = BufferMapType::WRITE;
            
            cmd_buf.write<CopyUniformCmdData>(cmd4);
            
            // Bind Per Frame Global Uniforms
            
			command_type = CommandBuffer::BindUniformBuffer;
            cmd_buf.write(command_type);
            
            BindUniformBufferCmdData cmd3;
            cmd3.buffer = _per_frame_buffer;
            cmd3.shader_type = ShaderType::VERTEX;
            cmd3.slot = PER_FRAME_UNIFORM_SLOT;
            
            cmd_buf.write<BindUniformBufferCmdData>(cmd3);
            
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
                        
						command_type = CommandBuffer::BindVertexArray;
                        cmd_buf.write(command_type);
                        
                        BindVertexArrayCmdData cmd5;
                        cmd5.vertex_array = draw_item.vertex_array;
                        
                        cmd_buf.write<BindVertexArrayCmdData>(cmd5);
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
                    ShaderProgram* program = shader_cache.load(key);
                    
                    if(last_program != program->m_resource_id)
                    {
                        last_program = program->m_resource_id;
                        
                        // Bind Shader Program
                        
						command_type = CommandBuffer::BindShaderProgram;
                        cmd_buf.write(command_type);
                        
                        BindShaderProgramCmdData cmd6;
                        cmd6.program = program;
                        
                        cmd_buf.write<BindShaderProgramCmdData>(cmd6);
                    }
                    
                    if(last_renderable != draw_item.renderable_id)
                    {
                        last_renderable = draw_item.renderable_id;
                        
                        // Copy Per Draw Uniforms
                        
						command_type = CommandBuffer::CopyUniformData;
                        cmd_buf.write(command_type);
                        
                        CopyUniformCmdData cmd8;
                        cmd8.buffer = _per_draw_buffer;
                        cmd8.data   = draw_item.uniforms;
                        cmd8.size   = sizeof(PerDrawUniforms);
                        cmd8.map_type = BufferMapType::WRITE;
                        
                        cmd_buf.write<CopyUniformCmdData>(cmd8);
                        
                        // Bind Per Draw Uniforms
                        
						command_type = CommandBuffer::BindUniformBuffer;
                        cmd_buf.write(command_type);
                        
                        BindUniformBufferCmdData cmd7;
                        cmd7.buffer = _per_draw_buffer;
                        cmd7.shader_type = ShaderType::VERTEX;
                        cmd7.slot = PER_DRAW_UNIFORM_SLOT;
                        
                        cmd_buf.write<BindUniformBufferCmdData>(cmd7);
                    }
                    
                    for (int l = 0; l < 5 ; l++)
                    {
                        if(draw_item.material)
                        {
                            if(draw_item.material->texture_maps[l])
                            {
                                // Bind Sampler State
                                
								command_type = CommandBuffer::BindSamplerState;
                                cmd_buf.write(command_type);
                                
                                BindSamplerStateCmdData cmd9;
                                cmd9.state = draw_item.material->sampler;
                                cmd9.slot = l;
                                cmd9.shader_type = ShaderType::PIXEL;
                                
                                cmd_buf.write<BindSamplerStateCmdData>(cmd9);
                                
                                if(last_texture != draw_item.material->texture_maps[l]->m_resource_id)
                                {
                                    last_texture = draw_item.material->texture_maps[l]->m_resource_id;
                                    
                                    // Bind Textures
                                    
									command_type = CommandBuffer::BindTexture;
                                    cmd_buf.write(command_type);
                                    
                                    BindTextureCmdData cmd10;
                                    cmd10.texture = draw_item.material->texture_maps[l];
                                    cmd10.slot = l;
                                    cmd10.shader_type = ShaderType::PIXEL;
                                    
                                    cmd_buf.write<BindTextureCmdData>(cmd10);
                                }
                            }
                        }
                    }
                    
                    // Draw
                    
					command_type = CommandBuffer::DrawIndexedBaseVertex;
                    cmd_buf.write(command_type);
                    
                    DrawIndexedBaseVertexCmdData cmd11;
                    cmd11.base_index = draw_item.base_index;
                    cmd11.base_vertex = draw_item.base_vertex;
                    cmd11.index_count = draw_item.index_count;
                    
                    cmd_buf.write<DrawIndexedBaseVertexCmdData>(cmd11);
                }
            }
            else if(render_pass->geometry_type == GeometryType::SKY)
            {
                // Bind Vertex Array
                
				command_type = CommandBuffer::BindVertexArray;
                cmd_buf.write(command_type);
               
                BindVertexArrayCmdData cmd5;
                cmd5.vertex_array = render_system._skydome_item.vertex_array;
                
                cmd_buf.write<BindVertexArrayCmdData>(cmd5);
                
                ShaderKey key;
                
                key.encode_mesh_type(render_system._skydome_item.type);
                key.encode_renderpass_id(render_pass->pass_id);
                
                // Send to rendering thread pool
                ShaderProgram* program = shader_cache.load(key);
                
                // Bind Shader Program
                
				command_type = CommandBuffer::BindShaderProgram;
                cmd_buf.write(command_type);
                
                BindShaderProgramCmdData cmd6;
                cmd6.program = program;
                
                cmd_buf.write<BindShaderProgramCmdData>(cmd6);
                
                // Copy Per Frame data
                
				command_type = CommandBuffer::CopyUniformData;
                cmd_buf.write(command_type);
                
                CopyUniformCmdData cmd4;
                cmd4.buffer = _per_frame_sky_buffer;
                cmd4.data   = per_frame_sky;
                cmd4.size   = sizeof(PerFrameSkyUniforms);
                cmd4.map_type = BufferMapType::WRITE;
                
                cmd_buf.write<CopyUniformCmdData>(cmd4);
                
                // Bind Per Frame Sky Global Uniforms
                
				command_type = CommandBuffer::BindUniformBuffer;
                cmd_buf.write(command_type);
                
                BindUniformBufferCmdData cmd3;
                cmd3.buffer = _per_frame_sky_buffer;
                cmd3.shader_type = ShaderType::VERTEX;
                cmd3.slot = PER_FRAME_UNIFORM_SLOT;
                
                cmd_buf.write<BindUniformBufferCmdData>(cmd3);
                
                // Bind Sampler State
                
				command_type = CommandBuffer::BindSamplerState;
                cmd_buf.write(command_type);
                
                BindSamplerStateCmdData cmd9;
                cmd9.state = render_system._sky_cmp->sampler;
                cmd9.slot = 0;
                cmd9.shader_type = ShaderType::PIXEL;
                
                cmd_buf.write<BindSamplerStateCmdData>(cmd9);
                
                // Bind Cubemap
                
				command_type = CommandBuffer::BindTexture;
                cmd_buf.write(command_type);
                
                BindTextureCmdData cmd10;
                cmd10.texture = render_system._sky_cmp->texture;
                cmd10.slot = 0;
                cmd10.shader_type = ShaderType::PIXEL;
                
                cmd_buf.write<BindTextureCmdData>(cmd10);
                
                // Draw
                
				command_type = CommandBuffer::DrawIndexedBaseVertex;
                cmd_buf.write(command_type);
                
                DrawIndexedBaseVertexCmdData cmd11;
                cmd11.base_index = render_system._skydome_item.base_index;
                cmd11.base_vertex = render_system._skydome_item.base_vertex;
                cmd11.index_count = render_system._skydome_item.index_count;
                
                cmd_buf.write<DrawIndexedBaseVertexCmdData>(cmd11);
            }
            else if(render_pass->geometry_type == GeometryType::QUAD)
            {
                
            }
        }
    }
    
    cmd_buf.end();
    
    // reset
    scene_view._num_items = 0;
    
    TERMINUS_END_CPU_PROFILE;
}

void submit_gpu_upload_task(Task& task)
{
	Context& context = global::get_context();
	// queue task into rendering thread.
	context._rendering_thread.enqueue_upload_task(task);
	sync::wait_for_loader_wakeup();
}

TERMINUS_END_NAMESPACE
