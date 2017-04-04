#pragma once

#include <Core/config.h>

#if defined(TERMINUS_OPENGL)

#include <vector>
#include <graphics/render_common.h>
#include <resource/asset_common.h>
#include <core/types.h>
#include <core/global.h>
#include <container/packed_array.h>
#include <platform/platform.h>
#include <memory/pool_allocator.h>
#include <graphics/graphics_types_gl.h>

#define MAX_TEXTURE_1D 1
#define MAX_TEXTURE_2D 1
#define MAX_TEXTURE_3D 1024
#define MAX_TEXTURE_CUBE 64
#define MAX_FRAMEBUFFER 64
#define MAX_VERTEX_BUFFER 64
#define MAX_INDEX_BUFFER 64
#define MAX_UNIFORM_BUFFER 64
#define MAX_VERTEX_ARRAY 64
#define MAX_SHADER_PROGRAM 256
#define MAX_SHADER 256
#define MAX_SAMPLER_STATE 16
#define MAX_RASTERIZER_STATE 16
#define MAX_DEPTH_STENCIL_STATE 16

namespace terminus
{
		class RenderDevice
		{

		private:
			// Stateful Object References for Window mode switching. GL only.
			PackedArray<VertexArray*, MAX_VERTEX_ARRAY>	m_VertexArrayPool;
			PackedArray<Framebuffer*, MAX_FRAMEBUFFER> 	m_FramebufferPool;
            
            // Resource IDs
            
            uint16 m_texture_res_id;
            uint16 m_buffer_res_id;
            uint16 m_framebuffer_res_id;
            uint16 m_vertex_array_res_id;
            uint16 m_shader_program_res_id;
            uint16 m_sampler_res_id;

		public:

			RenderDevice();
			~RenderDevice();
			void Initialize();
			void Shutdown();

			// Object Creation
			Texture1D*		 CreateTexture1D(uint16 width,
                                             void* data,
                                             TextureFormat format,
                                             bool generateMipmaps = true,
                                             uint mipMapLevels = 10);
			Texture2D*		 CreateTexture2D(uint16 width,
                                             uint16 height,
                                             void* data,
                                             TextureFormat format,
                                             bool createRenderTargetView,
                                             bool generateMipmaps = true,
                                             uint mipMapLevels = 10);
            void		 CreateTexture2D(Texture2D* texture,
                                         uint16 width,
                                         uint16 height,
                                         void* data,
                                         TextureFormat format,
                                         bool createRenderTargetView,
                                         bool generateMipmaps = true,
                                         uint mipMapLevels = 10);
			Texture3D*		 CreateTexture3D(uint16 width,
				uint16 height,
				uint16 depth,
				void* data,
				TextureFormat format,
				bool generateMipmaps = true,
				uint mipMapLevels = 10);
            
            
			TextureCube*	 CreateTextureCube(TextureCubeCreateDesc desc);
			VertexBuffer*	 CreateVertexBuffer(void* data,
				uint size,
				BufferUsageType usageType);
			IndexBuffer*	 CreateIndexBuffer(void* data,
				uint size,
				BufferUsageType usageType);
			UniformBuffer*   CreateUniformBuffer(void* data,
				uint size,
				BufferUsageType usageType);
			VertexArray*     CreateVertexArray(VertexBuffer* vertexBuffer,
				IndexBuffer* indexBuffer,
				InputLayoutType layoutType,
				InputLayout* layout = nullptr);
			RasterizerState* CreateRasterizerState(CullMode cullMode = CullMode::BACK,
				FillMode fillMode = FillMode::SOLID,
				bool frontWindingCCW = true,
				bool multisample = false,
				bool scissor = false);
			SamplerState*	 CreateSamplerState(TextureFilteringMode minFilter,
				TextureFilteringMode magFilter,
				TextureWrapMode wrapModeU,
				TextureWrapMode wrapModeV,
				TextureWrapMode wrapModeW,
				float maxAnisotropy = 1.0f,
				float borderRed = 0.0f,
				float borderGreen = 0.0f,
				float borderBlue = 0.0f,
				float borderAlpha = 0.0f);
			Framebuffer*	 CreateFramebuffer();
			Shader*			 CreateShader(ShaderType type,
				const char* shaderSource);
			ShaderProgram*   CreateShaderProgram(Shader* vertexShader,
				Shader* pixelShader,
				Shader* geometryShader = nullptr,
				Shader* controlShader = nullptr,
				Shader* evaluationShader = nullptr);
			DepthStencilState* CreateDepthStencilState(bool enableDepthTest = true,
				bool enableStencilTest = false,
				bool depthMask = true,
				ComparisonFunction depthComparisonFunction = ComparisonFunction::LESS,
				StencilOperation frontStencilFail = StencilOperation::REPLACE,
				StencilOperation frontStencilPassDepthFail = StencilOperation::REPLACE,
				StencilOperation frontStencilPassDepthPass = StencilOperation::REPLACE,
				ComparisonFunction frontStencilComparisonFunction = ComparisonFunction::LESS,
				StencilOperation backStencilFail = StencilOperation::REPLACE,
				StencilOperation backStencilPassDepthFail = StencilOperation::REPLACE,
				StencilOperation backStencilPassDepthPass = StencilOperation::REPLACE,
				ComparisonFunction backStencilComparisonFunction = ComparisonFunction::LESS,
				uint stencilMask = 0xFF);
			void AttachRenderTarget(Framebuffer* framebuffer, Texture* renderTarget);
			void AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget);

			// Object Destruction
			void DestroyTexture1D(Texture1D* texture);
			void DestroyTexture2D(Texture2D* texture);
			void DestroyTexture3D(Texture3D* texture);
			void DestroyTextureCube(TextureCube* texture);
			void DestroyVertexBuffer(VertexBuffer* buffer);
			void DestroyIndexBuffer(IndexBuffer* buffer);
			void DestroyUniformBuffer(UniformBuffer* buffer);
			void DestroyVertexArray(VertexArray* vertexArray);
			void DestroyRasterizerState(RasterizerState* state);
			void DestroySamplerState(SamplerState* state);
			void DestroyDepthStencilState(DepthStencilState* state);
			void DestroyFramebuffer(Framebuffer* framebuffer);
			void DestroyShader(Shader* shader);
			void DestoryShaderProgram(ShaderProgram* program);

			// Object Use
			void  BindTexture(Texture* texture,
				ShaderType shaderStage,
				uint bufferSlot);
			void  BindUniformBuffer(UniformBuffer* uniformBuffer,
				ShaderType shaderStage,
				uint bufferSlot);
			void  BindRasterizerState(RasterizerState* state);
			void  BindSamplerState(SamplerState* state,
				ShaderType shaderStage,
				uint slot);
			void  BindVertexArray(VertexArray* vertexArray);
			void  BindFramebuffer(Framebuffer* framebuffer);
			void  BindDepthStencilState(DepthStencilState* state);
			void  BindShaderProgram(ShaderProgram* program);
			void* MapBuffer(Buffer* buffer, BufferMapType type);
			void  UnmapBuffer(Buffer* buffer);
			void  SetPrimitiveType(DrawPrimitive primitive);
			void  ClearFramebuffer(FramebufferClearTarget clearTarget, Vector4 clearColor);
			void  SetViewport(int width, int height, int topLeftX, int topLeftY);
			void  SwapBuffers();
			
			// Stateless Methods
			void Draw(int firstIndex,
				int count);
			void DrawIndexed(int indexCount);
			void DrawIndexedBaseVertex(int indexCount,
				unsigned int baseIndex,
				unsigned int baseVertex);
			void DrawInstanced();
			void DrawIndexedInstanced();

			inline Framebuffer* GetFramebufferFromPool(String name)
			{
				return _framebuffer_map[name];
			}

			inline Texture2D* GetRenderTargetFromPool(String name)
			{
				return _render_target_map[name];
			}

			inline void AddToRenderTargetPool(String name, Texture2D* texture)
			{
				_render_target_map[name] = texture;
			}

			inline void AddToFramebufferPool(String name, Framebuffer* framebuffer)
			{
				_framebuffer_map[name] = framebuffer;
			}

		private:
			Framebuffer*							 _current_framebuffer;
			Platform*								 _platform;
			GLenum									 _primitive_type;
			ShaderProgram*							 _current_program;
			GLuint									 _last_sampler_location;
			int										 _drawable_width;
			int										 _drawable_height;
			int										 _width;
			int										 _height;
			std::unordered_map<String, Texture2D*>	 _render_target_map;
			std::unordered_map<String, Framebuffer*> _framebuffer_map;
		};
} // namespace terminus

#endif
