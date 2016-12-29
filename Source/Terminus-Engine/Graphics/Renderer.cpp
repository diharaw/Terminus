#include "Renderer.h"
#include "../Core/Config.h"
#include "../GUI/ImGuiBackend.h"

namespace Terminus { namespace Graphics {
  
    Renderer::Renderer()
    {
        
    }
    
    Renderer::~Renderer()
    {
        
    }
    
    void Renderer::Submit(RenderDevice& device)
    {
        GraphicsQueue& queue = GetGraphicsQueueBack();
        
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
                        break;
                    }
                    case CommandType::BindShaderProgram:
                    {
                        BindFramebufferCmdData _cmd_data;
                        _cmd_buf.Read<BindFramebufferCmdData>(_cmd_data);
                        device.BindFramebuffer(_cmd_data.framebuffer);
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
#endif
        
        // Swap Queues
        m_front_queue_index = !m_front_queue_index;
        
        device.SwapBuffers();
    }

    GraphicsQueue& Renderer::GetGraphicsQueueFront()
    {
        return m_graphics_queues[m_front_queue_index];
    }
    
    GraphicsQueue& Renderer::GetGraphicsQueueBack()
    {
        return m_graphics_queues[!m_front_queue_index];
    }
    
} }
