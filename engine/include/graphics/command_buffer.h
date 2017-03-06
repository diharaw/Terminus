#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <core/types.h>
#include <core/global.h>
#include <graphics/render_device.h>
#include <memory/allocator.h>

#define COMMAND_BUFFER_SIZE 1
#define COMMAND_BUFFER_SIZE_BYTES COMMAND_BUFFER_SIZE*MB_IN_BYTES

namespace terminus
{
    enum class CommandType
    {
        Draw = 0,
        DrawIndexed,
        DrawIndexedBaseVertex,
        BindFramebuffer,
        BindShaderProgram,
        BindVertexArray,
        BindUniformBuffer,
        BindSamplerState,
        BindRasterizerState,
        BindDepthStencilState,
        BindTexture2D,
        CopyUniformData,
        ClearFramebuffer,
        End
    };
    
    struct DrawCmdData
    {
        int first_index;
        int count;
    };
    
    struct DrawIndexedCmdData
    {
        int index_count;
    };
    
    struct DrawIndexedBaseVertexCmdData
    {
        int index_count;
        unsigned int base_index;
        unsigned int base_vertex;
    };
    
    struct BindFramebufferCmdData
    {
        Framebuffer* framebuffer;
    };
    
    struct BindShaderProgramCmdData
    {
        ShaderProgram* program;
    };
    
    struct BindVertexArrayCmdData
    {
        VertexArray* vertex_array;
    };
    
    struct ClearFramebufferCmdData
    {
        Vector4 clear_color;
        FramebufferClearTarget clear_target;
    };
    
    struct BindUniformBufferCmdData
    {
        UniformBuffer* buffer;
        ShaderType shader_type;
        uint slot;
    };
    
    struct CopyUniformCmdData
    {
        UniformBuffer* buffer;
        BufferMapType map_type;
        void* data;
        size_t size;
    };
    
    struct BindRasterizerStateData
    {
        RasterizerState* state;
    };
    
    struct BindDepthStencilStateData
    {
        DepthStencilState* state;
    };
    
    struct BindTexture2DCmdData
    {
        Texture2D* texture;
        ShaderType shader_type;
        int           slot;
    };
    
    struct BindSamplerStateCmdData
    {
        SamplerState* state;
        ShaderType shader_type;
        int           slot;
    };
    
    struct CommandBuffer
    {
        void*  m_memory;
        void*  m_pos;
        size_t m_total;
        size_t m_used;
        
        CommandBuffer()
        {
            m_total = COMMAND_BUFFER_SIZE_BYTES;
            m_used = 0;
        }
        
        ~CommandBuffer()
        {
            m_memory = nullptr;
            m_pos = nullptr;
            m_total = 0;
            m_used = 0;
        }
        
        inline void WriteEnd()
        {
            Write(CommandType::End);
            m_pos = m_memory;
        }
        
        inline void Write(CommandType cmd)
        {
            Write(&cmd, sizeof(CommandType));
        }
        
        inline void Write(void* cmd, size_t size)
        {
            assert((m_used + size) <= m_total);
            memcpy(m_pos, cmd, size);
            Move(size);
            m_used += size;
        }
        
        template<typename T>
        inline void Write(T* cmd)
        {
            size_t size = sizeof(T);
            assert((m_used + size) <= m_total);
            memcpy(m_pos, cmd, size);
            Move(size);
            m_used += size;
        }
        
        inline void ReadCmd(CommandType& cmd)
        {
            Read<CommandType>(cmd);
        }
        
        inline void Read(void* data, size_t size)
        {
            // TODO : assert position
            memcpy(data, m_pos, size);
            Move(size);
        }

		template<typename T>
		inline T* Read()
		{
			T* cmd = (T*)m_pos;
			Move(sizeof(T));
			return cmd;
		}
        
        inline void Move(size_t size)
        {
            m_pos = AllocatorUtility::Add(m_pos, size);
        }
        
        template<typename T>
        inline void Read(T& data)
        {
            Read(&data, sizeof(T));
        }
        
        inline void Clear()
        {
            m_used = 0;
            m_pos = m_memory;
        }
        
    };
}

#endif
