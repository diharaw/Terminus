#include "RenderDevice.h"

#ifdef TERMINUS_DIRECT3D11

namespace Terminus { namespace Graphics {

	RenderDevice::RenderDevice()
	{

	}
	RenderDevice::~RenderDevice()
	{

	}

	Texture1D* RenderDevice::CreateTexture1D(void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{

	}

	Texture2D* RenderDevice::CreateTexture2D(void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{

	}

	Texture3D* RenderDevice::CreateTexture3D(void* data,
											 TextureFormat format,
											 bool generateMipmaps,
											 uint mipMapLevels)
	{

	}

	TextureCube* RenderDevice::CreateTextureCube(void* data,
												 TextureFormat format,
												 bool generateMipmaps,
												 uint mipMapLevels)
	{

	}

	VertexBuffer* RenderDevice::CreateVertexBuffer(void* data,
												   uint size,
												   BufferUsageType usageType)
	{

	}

	IndexBuffer* RenderDevice::CreateIndexBuffer(void* data,
												 uint size,
												 BufferUsageType usageType)
	{

	}

	UniformBuffer* RenderDevice::CreateUniformBuffer(void* data,
													 uint size,
													 BufferUsageType usageType)
	{

	}

	VertexArray* RenderDevice::CreateVertexArray(VertexBuffer* vertexBuffer,
												 IndexBuffer* indexBuffer,
												 InputLayoutType layoutType,
												 InputLayout* layout)
	{

	}

	RasterizerState* RenderDevice::CreateRasterizerState(CullMode cullMode,
														 FillMode fillMode,
														 bool frontWindingCCW,
														 bool multisample,
														 bool scissor)
	{

	}

	SamplerState* RenderDevice::CreateSamplerState(TextureFilteringMode minFilter,
												   TextureFilteringMode magFilter,
												   TextureWrapMode wrapModeU,
												   TextureWrapMode wrapModeV,
												   TextureWrapMode wrapModeW,
												   float maxAnisotropy,
												   float borderRed,
												   float borderGreen,
												   float borderBlue,
												   float borderAlpha)
	{

	}

	Framebuffer* RenderDevice::CreateFramebuffer()
	{

	}

	Shader* RenderDevice::CreateShader(ShaderType type,
									   const char* shaderSource)
	{

	}

	ShaderProgram* RenderDevice::CreateShaderProgram(Shader* vertexShader,
													 Shader* pixelShader,
													 Shader* geometryShader,
													 Shader* controlShader,
													 Shader* evaluationShader)
	{

	}

	DepthStencilState* RenderDevice::CreateDepthStencilState(bool enableDepthTest,
															 bool enableStencilTest,
															 bool depthMask,
															 ComparisonFunction depthComparisonFunction,
															 StencilOperation frontStencilFail,
															 StencilOperation frontStencilPassDepthFail,
															 StencilOperation frontStencilPassDepthPass,
															 ComparisonFunction frontStencilComparisonFunction,
															 StencilOperation backStencilFail,
															 StencilOperation backStencilPassDepthFail,
															 StencilOperation backStencilPassDepthPass,
															 ComparisonFunction backStencilComparisonFunction,
															 uint stencilMask)
	{

	}

	void RenderDevice::AttachRenderTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{

	}

	void RenderDevice::AttachDepthStencilTarget(Framebuffer* framebuffer, Texture* renderTarget)
	{

	}

	void RenderDevice::DestroyTexture1D(Texture1D* texture)
	{

	}

	void RenderDevice::DestroyTexture2D(Texture2D* texture)
	{

	}

	void RenderDevice::DestroyTexture3D(Texture3D* texture)
	{

	}

	void RenderDevice::DestroyTextureCube(TextureCube* texture)
	{


	}

	void RenderDevice::DestroyVertexBuffer(VertexBuffer* buffer)
	{

	}

	void RenderDevice::DestroyIndexBuffer(IndexBuffer* buffer)
	{

	}

	void RenderDevice::DestroyUniformBuffer(UniformBuffer* buffer)
	{

	}

	void RenderDevice::DestroyRasterizerState(RasterizerState* state)
	{

	}

	void RenderDevice::DestroySamplerState(SamplerState* state)
	{

	}

	void RenderDevice::DestroyDepthStencilState(DepthStencilState* state)
	{


	}

	void RenderDevice::DestroyFramebuffer(Framebuffer* framebuffer)
	{

	}

	void RenderDevice::DestroyShader(Shader* shader)
	{

	}

	void RenderDevice::DestoryShaderProgram(ShaderProgram* program)
	{

	}

	void RenderDevice::BindTexture(Texture* texture,
								   ShaderType shaderStage)
	{

	}

	void RenderDevice::BindUniformBuffer(UniformBuffer* uniformBuffer,
										 ShaderType shaderStage,
										 uint bufferSlot)
	{

	}

	void RenderDevice::BindRasterizerState(RasterizerState* state)
	{

	}

	void RenderDevice::BindSamplerState(SamplerState* state,
										ShaderType shaderStage,
										uint slot)
	{

	}

	void RenderDevice::BindFramebuffer(Framebuffer* framebuffer)
	{

	}

	void RenderDevice::BindDepthStencilState(DepthStencilState* state)
	{

	}

	void RenderDevice::BindShaderProgram(ShaderProgram* program)
	{

	}

	void* RenderDevice::MapBuffer(Buffer* buffer, BufferMapType type)
	{

	}

	void RenderDevice::UnmapBuffer(Buffer* buffer)
	{

	}

	void RenderDevice::Draw(int firstIndex,
							int count)
	{

	}

	void RenderDevice::DrawIndexed(int indexCount)
	{

	}

	void RenderDevice::DrawIndexedBaseVertex(int indexCount,
											 unsigned int baseIndex,
											 unsigned int baseVertex)
	{

	}

	void RenderDevice::DrawInstanced()
	{


	}

	void RenderDevice::DrawIndexedInstanced()
	{

	}

} }

#endif