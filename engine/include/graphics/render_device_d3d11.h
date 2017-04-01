#pragma once

#include <Core/config.h>

#if defined(TERMINUS_DIRECT3D11)

#include <vector>
#include <graphics/render_common.h>
#include <resource/asset_common.h>
#include <core/types.h>
#include <container/packed_array.h>
#include <platform/platform.h>
#include <memory/pool_allocator.h>
#include <graphics/graphics_types_d3d11.h>

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
			// Allocators 
			PoolAllocator* m_Texture1DAllocator;
			PoolAllocator* m_Texture2DAllocator;
			PoolAllocator* m_Texture3DAllocator;
			PoolAllocator* m_TextureCubeAllocator;
			PoolAllocator* m_VertexBufferAllocator;
			PoolAllocator* m_IndexBufferAllocator;
			PoolAllocator* m_UniformBufferAllocator;
			PoolAllocator* m_VertexArrayAllocator;
			PoolAllocator* m_ShaderAllocator;
			PoolAllocator* m_ShaderProgramAllocator;
			PoolAllocator* m_FramebufferAllocator;
			PoolAllocator* m_RasterizerStateAllocator;
			PoolAllocator* m_DepthStencilStateAllocator;
			PoolAllocator* m_SamplerStateAllocator;

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
			Texture2D* CreateTexture2D(Texture2D* texture,
									   uint16 width,
									   uint16 height,
									   void* data,
									   TextureFormat format,
									   bool createRenderTargetView,
									   bool generateMipmaps,
									   uint mipMapLevels);
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

			// Getters
			inline ID3D11Device* GetD3D11Device()
			{
				return m_device;
			}

			inline ID3D11DeviceContext* GetD3D11DeviceContext()
			{
				return m_device_context;
			}

			inline Framebuffer* GetFramebufferFromPool(String name)
			{
				return m_framebuffer_map[name];
			}

			inline Texture2D* GetRenderTargetFromPool(String name)
			{
				return m_render_target_map[name];
			}

			inline void AddToRenderTargetPool(String name, Texture2D* texture)
			{
				m_render_target_map[name] = texture;
			}

			inline void AddToFramebufferPool(String name, Framebuffer* framebuffer)
			{
				m_framebuffer_map[name] = framebuffer;
			}

		private:
			DXGI_FORMAT get_dxgi_format(TextureFormat format);

		private:

			bool InitializeAPI();
			ID3D10Blob* CreateStubShaderByteCodeFromLayout(InputLayout _layout);

			bool				     m_vsync;
			int						 m_video_card_memory;
			Framebuffer*			 m_current_framebuffer;
			Framebuffer*			 m_default_framebuffer;
			Texture2D*				 m_default_render_target;
			Texture2D*				 m_default_depth_target;
			char					 m_video_card_desc[128];
			std::unordered_map<String, Texture2D*> m_render_target_map;
			std::unordered_map<String, Framebuffer*> m_framebuffer_map;
			float					 _window_width;
			float					 _window_height;

			D3D11_PRIMITIVE_TOPOLOGY m_primitive_type;
			ID3D11Device*			 m_device;
			ID3D11DeviceContext*	 m_device_context;
			IDXGISwapChain*			 m_swap_chain;

			// Resource IDs

			uint16 m_texture_res_id;
			uint16 m_buffer_res_id;
			uint16 m_framebuffer_res_id;
			uint16 m_vertex_array_res_id;
			uint16 m_shader_program_res_id;
			uint16 m_sampler_res_id;
		};
} // namespace terminus

#endif
