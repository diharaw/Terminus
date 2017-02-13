#include <Graphics/renderer.h>
#include <Graphics/imgui_backend.h>
#include <Core/config.h>
#include <Core/context.h>

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
        
        _per_frame_buffer = device.CreateUniformBuffer(nullptr, sizeof(PerFrameUniforms), BufferUsageType::DYNAMIC);
        _per_draw_buffer = device.CreateUniformBuffer(nullptr, sizeof(PerDrawUniforms), BufferUsageType::DYNAMIC);
        _per_draw_material_buffer = device.CreateUniformBuffer(nullptr, sizeof(PerDrawMaterialUniforms), BufferUsageType::DYNAMIC);
        _per_draw_bone_offsets_buffer = device.CreateUniformBuffer(nullptr, sizeof(PerDrawBoneOffsetUniforms), BufferUsageType::DYNAMIC);
        _rasterizer_state = device.CreateRasterizerState();
        _depth_stencil_state = device.CreateDepthStencilState();
    }
    
    void Renderer::shutdown()
    {
        RenderDevice& device = context::get_render_device();
        
        device.DestroyUniformBuffer(_per_frame_buffer);
        device.DestroyUniformBuffer(_per_draw_buffer);
        device.DestroyUniformBuffer(_per_draw_material_buffer);
        device.DestroyUniformBuffer(_per_draw_bone_offsets_buffer);
        device.DestroyRasterizerState(_rasterizer_state);
        device.DestroyDepthStencilState(_depth_stencil_state);
    }
    
    void Renderer::submit()
    {
        RenderDevice& device = context::get_render_device();
        
        GraphicsQueue& queue = graphics_queue_back();
        
        // temp
        device.BindRasterizerState(_rasterizer_state);
        device.BindDepthStencilState(_depth_stencil_state);
        
        if(queue.m_num_cmd_buf == 0)
            device.ClearFramebuffer(FramebufferClearTarget::ALL, Vector4(0.3f, 0.3f, 0.3f, 1.0f));
        
        for (int i = 0; i < queue.m_num_cmd_buf; i++)
        {
            CommandBuffer& _cmd_buf = queue.m_cmd_buf[i];
            CommandType _cmd;
            bool is_done = false;
            
            do
            {
                _cmd_buf.ReadCmd(_cmd);
                
                switch(_cmd)
                {
                    case CommandType::Draw:
                    {
                        DrawCmdData _cmd_data;
                        _cmd_buf.Read<DrawCmdData>(_cmd_data);
                        device.Draw(_cmd_data.first_index, _cmd_data.count);
                        break;
                    }
                    case CommandType::DrawIndexed:
                    {
                        DrawIndexedCmdData _cmd_data;
                        _cmd_buf.Read<DrawIndexedCmdData>(_cmd_data);
                        device.DrawIndexed(_cmd_data.index_count);
                        break;
                    }
                    case CommandType::DrawIndexedBaseVertex:
                    {
                        // temp
                        device.SetPrimitiveType(DrawPrimitive::TRIANGLES);
                        
                        DrawIndexedBaseVertexCmdData _cmd_data;
                        _cmd_buf.Read<DrawIndexedBaseVertexCmdData>(_cmd_data);
                        device.DrawIndexedBaseVertex(_cmd_data.index_count, _cmd_data.base_index, _cmd_data.base_vertex);
                        
                        break;
                    }
                    case CommandType::BindFramebuffer:
                    {
                        BindFramebufferCmdData _cmd_data;
                        _cmd_buf.Read<BindFramebufferCmdData>(_cmd_data);
                        device.BindFramebuffer(_cmd_data.framebuffer);
                        
                        // temp
                        device.SetViewport(0, 0, 0, 0);
                        
                        break;
                    }
                    case CommandType::BindShaderProgram:
                    {
                        BindShaderProgramCmdData _cmd_data;
                        _cmd_buf.Read<BindShaderProgramCmdData>(_cmd_data);
                        device.BindShaderProgram(_cmd_data.program);
                        break;
                    }
                    case CommandType::BindSamplerState:
                    {
                        BindSamplerStateCmdData _cmd_data;
                        _cmd_buf.Read<BindSamplerStateCmdData>(_cmd_data);
                        device.BindSamplerState(_cmd_data.state, _cmd_data.shader_type, _cmd_data.slot);
                        break;
                    }
                    case CommandType::BindVertexArray:
                    {
                        BindVertexArrayCmdData _cmd_data;
                        _cmd_buf.Read<BindVertexArrayCmdData>(_cmd_data);
                        device.BindVertexArray(_cmd_data.vertex_array);
                        break;
                    }
                    case CommandType::BindUniformBuffer:
                    {
                        BindUniformBufferCmdData _cmd_data;
                        _cmd_buf.Read<BindUniformBufferCmdData>(_cmd_data);
                        device.BindUniformBuffer(_cmd_data.buffer, _cmd_data.shader_type, _cmd_data.slot);
                        break;
                    }
                    case CommandType::BindRasterizerState:
                    {
                        BindRasterizerStateData _cmd_data;
                        _cmd_buf.Read<BindRasterizerStateData>(_cmd_data);
                        device.BindRasterizerState(_cmd_data.state);
                        break;
                    }
                    case CommandType::BindDepthStencilState:
                    {
                        BindDepthStencilStateData _cmd_data;
                        _cmd_buf.Read<BindDepthStencilStateData>(_cmd_data);
                        device.BindDepthStencilState(_cmd_data.state);
                        break;
                    }
                    case CommandType::BindTexture2D:
                    {
                        BindTexture2DCmdData _cmd_data;
                        _cmd_buf.Read<BindTexture2DCmdData>(_cmd_data);
                        device.BindTexture(_cmd_data.texture, _cmd_data.shader_type, _cmd_data.slot);
                        break;
                    }
                    case CommandType::CopyUniformData:
                    {
                        CopyUniformCmdData _cmd_data;
                        _cmd_buf.Read<CopyUniformCmdData>(_cmd_data);
                        
                        void* ptr = device.MapBuffer(_cmd_data.buffer, _cmd_data.map_type);
                        memcpy(ptr, _cmd_data.data, _cmd_data.size);
                        device.UnmapBuffer(_cmd_data.buffer);
                        
                        break;
                    }
                    case CommandType::ClearFramebuffer:
                    {
                        ClearFramebufferCmdData _cmd_data;
                        _cmd_buf.Read<ClearFramebufferCmdData>(_cmd_data);
                        device.ClearFramebuffer(_cmd_data.clear_target, _cmd_data.clear_color);
                        
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
        }
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::render();
        ImGuiBackend::new_frame();
#endif
        device.SwapBuffers();
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
