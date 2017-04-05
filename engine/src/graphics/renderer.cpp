#include <graphics/renderer.h>
#include <graphics/imgui_backend.h>
#include <core/config.h>
#include <core/context.h>

#include <utility/profiler.h>

namespace terminus
{
    Renderer::Renderer()
    {
        
    }
    
    Renderer::~Renderer()
    {
        
    }
    
    void Renderer::initialize()
    {
        RenderDevice& device = context::get_render_device();
        
        BufferCreateDesc desc;
        
        desc.data = nullptr;
        desc.usage_type = BufferUsageType::DYNAMIC;
        desc.size = sizeof(PerFrameUniforms);
        
        _per_frame_buffer = device.create_uniform_buffer(desc);
        
        desc.size = sizeof(PerDrawUniforms);
        
        _per_draw_buffer = device.create_uniform_buffer(desc);
        
        desc.size = sizeof(PerDrawMaterialUniforms);
        
        _per_draw_material_buffer = device.create_uniform_buffer(desc);
        
        desc.size = sizeof(PerDrawBoneOffsetUniforms);
        
        _per_draw_bone_offsets_buffer = device.create_uniform_buffer(desc);
        
        RasterizerStateCreateDesc rs_desc;
        _rasterizer_state = device.create_rasterizer_state(rs_desc);
        
        DepthStencilStateCreateDesc ds_desc;
        _depth_stencil_state = device.create_depth_stencil_state(ds_desc);

        ds_desc.depth_cmp_func = ComparisonFunction::LESS_EQUAL;
        
        _sky_ds = device.create_depth_stencil_state(ds_desc);
    }
    
    void Renderer::shutdown()
    {
        RenderDevice& device = context::get_render_device();
        
        device.destroy_uniform_buffer(_per_frame_buffer);
        device.destroy_uniform_buffer(_per_draw_buffer);
        device.destroy_uniform_buffer(_per_draw_material_buffer);
        device.destroy_uniform_buffer(_per_draw_bone_offsets_buffer);
        device.destroy_rasterizer_state(_rasterizer_state);
        device.destroy_depth_stencil_state(_depth_stencil_state);
    }
    
    void Renderer::submit()
    {
        RenderDevice& device = context::get_render_device();
        
        GraphicsQueue& queue = graphics_queue_back();
        
        // temp
        device.bind_rasterizer_state(_rasterizer_state);
        device.bind_depth_stencil_state(_depth_stencil_state);
        
        if(queue.m_num_cmd_buf == 0)
            device.clear_framebuffer(FramebufferClearTarget::ALL, Vector4(0.3f, 0.3f, 0.3f, 1.0f));
        
        for (int i = 0; i < queue.m_num_cmd_buf; i++)
        {
			//TERMINUS_BEGIN_GPU_PROFILE(gpu_execution);
			TERMINUS_BEGIN_CPU_PROFILE(gpu_dispatch);

            CommandBuffer& _cmd_buf = queue.m_cmd_buf[i];
            CommandType* _cmd;
            bool is_done = false;
 
            do
            {
                _cmd = _cmd_buf.Read<CommandType>();
				
                switch(*_cmd)
                {
                    case CommandType::Draw:
                    {
                        DrawCmdData* _cmd_data = _cmd_buf.Read<DrawCmdData>();
                        device.draw(_cmd_data->first_index, _cmd_data->count);
                        break;
                    }
                    case CommandType::DrawIndexed:
                    {
                        DrawIndexedCmdData* _cmd_data = _cmd_buf.Read<DrawIndexedCmdData>();
                        device.draw_indexed(_cmd_data->index_count);
                        break;
                    }
                    case CommandType::DrawIndexedBaseVertex:
                    {
                        // temp
                        device.set_primitive_type(DrawPrimitive::TRIANGLES);
                        
                        DrawIndexedBaseVertexCmdData* _cmd_data = _cmd_buf.Read<DrawIndexedBaseVertexCmdData>();
                        device.draw_indexed_base_vertex(_cmd_data->index_count, _cmd_data->base_index, _cmd_data->base_vertex);
                        
                        break;
                    }
                    case CommandType::BindFramebuffer:
                    {
                        BindFramebufferCmdData* _cmd_data = _cmd_buf.Read<BindFramebufferCmdData>();
                        device.bind_framebuffer(_cmd_data->framebuffer);
                        
                        // temp
                        device.set_viewport(0, 0, 0, 0);
                        
                        break;
                    }
                    case CommandType::BindShaderProgram:
                    {
                        BindShaderProgramCmdData* _cmd_data = _cmd_buf.Read<BindShaderProgramCmdData>();
                        device.bind_shader_program(_cmd_data->program);
                        break;
                    }
                    case CommandType::BindSamplerState:
                    {
						BindSamplerStateCmdData* _cmd_data = _cmd_buf.Read<BindSamplerStateCmdData>();
                        device.bind_sampler_state(_cmd_data->state, _cmd_data->shader_type, _cmd_data->slot);
                        break;
                    }
                    case CommandType::BindVertexArray:
                    {
                        BindVertexArrayCmdData* _cmd_data = _cmd_buf.Read<BindVertexArrayCmdData>();
                        device.bind_vertex_array(_cmd_data->vertex_array);
                        break;
                    }
                    case CommandType::BindUniformBuffer:
                    {
                        BindUniformBufferCmdData* _cmd_data = _cmd_buf.Read<BindUniformBufferCmdData>();
                        device.bind_uniform_buffer(_cmd_data->buffer, _cmd_data->shader_type, _cmd_data->slot);
                        break;
                    }
                    case CommandType::BindRasterizerState:
                    {
                        BindRasterizerStateData* _cmd_data = _cmd_buf.Read<BindRasterizerStateData>();
                        device.bind_rasterizer_state(_cmd_data->state);
                        break;
                    }
                    case CommandType::BindDepthStencilState:
                    {
                        BindDepthStencilStateData* _cmd_data = _cmd_buf.Read<BindDepthStencilStateData>();
                        device.bind_depth_stencil_state(_cmd_data->state);
                        break;
                    }
                    case CommandType::BindTexture2D:
                    {
                        BindTexture2DCmdData* _cmd_data = _cmd_buf.Read<BindTexture2DCmdData>();
                        device.bind_texture(_cmd_data->texture, _cmd_data->shader_type, _cmd_data->slot);
                        break;
                    }
                    case CommandType::CopyUniformData:
                    {
                        CopyUniformCmdData* _cmd_data = _cmd_buf.Read<CopyUniformCmdData>();
                        
                        void* ptr = device.map_buffer(_cmd_data->buffer, _cmd_data->map_type);
                        memcpy(ptr, _cmd_data->data, _cmd_data->size);
                        device.unmap_buffer(_cmd_data->buffer);
                        
                        break;
                    }
                    case CommandType::ClearFramebuffer:
                    {
                        ClearFramebufferCmdData* _cmd_data = _cmd_buf.Read<ClearFramebufferCmdData>();
                        device.clear_framebuffer(_cmd_data->clear_target, _cmd_data->clear_color);
                        
                        break;
                    }
                    case CommandType::End:
                    {
                        is_done = true;
                        _cmd_buf.Clear();
                        break;
                    }
                    default:
                    {
                        assert(false);
                        break;
                    }
                }

            } while(!is_done);

			TERMINUS_END_CPU_PROFILE
			//TERMINUS_END_GPU_PROFILE;
        }
    }
    
    void Renderer::swap()
    {
        // Swap Queues
        _front_queue_index = !_front_queue_index;
    }

    uint32 Renderer::create_command_buffer()
    {
        _graphics_queues[0].CreateCommandBuffer();
        return _graphics_queues[1].CreateCommandBuffer();
    }
    
    CommandBuffer& Renderer::command_buffer(uint32 index)
    {
        return _graphics_queues[_front_queue_index].m_cmd_buf[index];
    }
    
    GraphicsQueue& Renderer::graphics_queue_front()
    {
        return _graphics_queues[_front_queue_index];
    }
    
    GraphicsQueue& Renderer::graphics_queue_back()
    {
        return _graphics_queues[!_front_queue_index];
    }
    
    LinearAllocator* Renderer::uniform_allocator()
    {
        return _graphics_queues[_front_queue_index].m_allocator;
    }
    
}
