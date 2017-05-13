#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <core/types.h>
#include <core/global.h>
#include <graphics/render_device.h>

#define MAX_COMMAND_BUFFER_SIZE 1
#define MAX_COMMAND_BUFFER_SIZE_BYTES MAX_COMMAND_BUFFER_SIZE*MB_IN_BYTES

namespace terminus
{
    struct DrawCmdData
    {
        uint32_t first_index;
		uint32_t count;
    };
    
    struct DrawIndexedCmdData
    {
		uint32_t index_count;
    };
    
    struct DrawIndexedBaseVertexCmdData
    {
		uint32_t index_count;
        uint32_t base_index;
        uint32_t base_vertex;
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
        ShaderType	   shader_type;
		uint32_t	   slot;
    };
    
    struct CopyUniformCmdData
    {
        UniformBuffer* buffer;
        BufferMapType map_type;
        void* data;
        size_t size;
    };
    
    struct BindPipelineStateObjectData
    {
        PipelineStateObject* pso;
    };
    
    struct BindTextureCmdData
    {
        Texture*   texture;
        ShaderType shader_type;
		uint32_t   slot;
    };
    
    struct BindSamplerStateCmdData
    {
        SamplerState* state;
        ShaderType	  shader_type;
		uint32_t      slot;
    };
    
	struct CommandBuffer
	{
		enum CommandType
		{
			Draw = 0,
			DrawIndexed,
			DrawIndexedBaseVertex,
			BindFramebuffer,
			BindShaderProgram,
			BindVertexArray,
			BindUniformBuffer,
			BindSamplerState,
			BindPipelineStateObject,
			BindTexture,
			CopyUniformData,
			ClearFramebuffer,
			End,
			TotalCommands
		};

		CommandBuffer()
		{
			_pos = 0;
			_size = MAX_COMMAND_BUFFER_SIZE_BYTES;
			end();
		}

		inline void write(const void* data, uint32_t size)
		{
			assert(_size == MAX_COMMAND_BUFFER_SIZE_BYTES && "Called write outside start/finish?");
			assert(_pos < _size && "CommandBuffer::write error.");

			memcpy(&_buffer[_pos], data, size);
			_pos += size;
		}

		template<typename Type>
		inline void write(const Type& in)
		{
			align(alignof(Type));
			write(reinterpret_cast<const uint8_t*>(&in), sizeof(Type));
		}

		void read(void* data, uint32_t size)
		{
			assert(_pos < _size && "CommandBuffer::read error.");
			memcpy(data, &_buffer[_pos], size);
			_pos += size;
		}

		template<typename Type>
		inline void read(Type& in)
		{
			align(alignof(Type));
			read(reinterpret_cast<uint8_t*>(&in), sizeof(Type));
		}

		inline void align(uint32_t alignment)
		{
			const uint32_t mask = alignment - 1;
			const uint32_t pos = (_pos + mask) & (~mask);
			_pos = pos;
		}

		inline void reset()
		{
			_pos = 0;
		}

		inline void start()
		{
			_pos = 0;
			_size = MAX_COMMAND_BUFFER_SIZE_BYTES;
		}

		inline void end()
		{
			uint32_t cmd = End;
			write(cmd);
			_size = _pos;
			_pos = 0;
		}

		uint32_t _pos;
		uint32_t _size;
		uint32_t _id;
		uint8_t  _buffer[MAX_COMMAND_BUFFER_SIZE_BYTES];
	};
}

#endif
