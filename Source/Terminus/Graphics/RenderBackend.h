#pragma once


#ifndef RENDERBACKEND_H
#define RENDERBACKEND_H

#include "RenderPass.h"
#include <stdint.h>
#include <rapidjson.h>
#include "RenderCommon.h"
#include "../Types.h"

#define MAX_TEXTURE_2D 1024
#define MAX_TEXTURE_CUBE 64
#define MAX_FRAMEBUFFER 64
#define MAX_VERTEX_BUFFER 64
#define MAX_INDEX_BUFFER 64
#define MAX_UNIFORM_BUFFER 64
#define MAX_VERTEX_ARRAY 64
#define MAX_SHADER_PROGRAM 256
#define MAX_SHADER 256
#define MAX_SAMPLER_STATE 256
#define MAX_RASTERIZER_STATE 16
#define MAX_DEPTH_STENCIL_STATE 16

struct Texture2D;
struct TextureCube;
struct Framebuffer;
struct VertexArray;
struct VertexBuffer;
struct UniformBuffer;
struct IndexBuffer;
struct ShaderProgram;
struct Shader;
struct SamplerState;

#define GL_CHECK_ERROR(x)																				\
x;																										\
{																										\
	GLenum err(glGetError());																			\
																										\
	while (err != GL_NO_ERROR)																			\
	{																									\
		std::string error;																				\
																										\
		switch (err)																					\
		{																								\
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;						\
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;						\
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;						\
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;						\
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;		\
		}																								\
																										\
		std::cerr << "GL_" << error.c_str() << " - " << std::endl;										\
		err = glGetError();																				\
	}																									\
}																										\

namespace RenderBackend
{
    /**
     * Initializes Graphics API Context and Backend resources
     */
    extern void Initialize();
    /**
     * Swaps front and back buffers.
     */
    extern void SwapBuffers();
    /**
     * Creates a Texture2D instance
     * @param _Width Width of Texture.
     * @param _Height Height of Texture
     * @param _Data Pointer to data to be initialized with. Can be left as nullptr for Framebuffer attachments.
     * @param _MipMaps Should mipmaps be generated. Default is true.
     * @return ResourceHandle to created Resource
     */
    extern ResourceHandle CreateTexture2D(uint16_t _Width, uint16_t _Height, void* _Data = nullptr, bool _MipMaps = true);
    /**
     * Creates a Texture2D instance
     * @param _Width Width of Texture.
     * @param _Height Height of Texture
     * @param _Data Pointer to data to be initialized with. Can be left as nullptr for Framebuffer attachments.
     * @param _MipMaps Should mipmaps be generated. Default is true.
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateTextureCube();
    /**
     * Creates a Texture2D instance
     * @param _Width Width of Texture.
     * @param _Height Height of Texture
     * @param _Data Pointer to data to be initialized with. Can be left as nullptr for Framebuffer attachments.
     * @param _MipMaps Should mipmaps be generated. Default is true.
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateTextureCubeIndividual();
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateFramebuffer(ResourceHandle* _RenderTargets, int _Count, ResourceHandle* _DepthTarget);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateVertexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateIndexBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateUniformBuffer(void* _Data, unsigned int _Size, BufferUsageType _UsageType);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateVertexShader(char* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateGeometryShader(char* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateControlShader(char* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateEvaluationShader(char* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreatePixelShader(char* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateShaderProgram(ResourceHandle _Vertex,
											  ResourceHandle _Geometry,
											  ResourceHandle _TessellationControl,
											  ResourceHandle _TessellationEvalution,
											  ResourceHandle _Pixel);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern void SetTexture2DMipmap(ResourceHandle _Texture2D, int _Level, void* _Data);
    /**
     * Creates a Framebuffer with one or more Texture Handles as RenderTargets and one optional Texture Handle as a Depth Target.
     * @param _RenderTargets Array of Texture Resource Handles
     * @param _Count Number of Handles
     * @param _DepthTarget ResourceHandle to Texture to be used as Depth Target
     * @return ResourceHandle to created Resource
     */
	extern ResourceHandle CreateVertexArray(ResourceHandle _vertexBuffer,
                                            ResourceHandle _indexBuffer,
                                            InputLayoutType _layoutType = LAYOUT_STANDARD_VERTEX,
                                            InputLayout* _layout = nullptr);

    extern ResourceHandle CreateRasterizerState(CullMode _cullMode = CULL_BACK,
                                                FillMode _fillMode = FILL_SOLID,
                                                bool _frontWindingCCW = true,
                                                bool _multisample = false,
                                                bool _scissor = false);
    
    extern ResourceHandle CreateDepthStencilState(bool _enableDepthTest = true,
                                                  bool _enableStencilTest = false,
                                                  bool _depthMask = true,
                                                  ComparisonFunction _depthComparisonFunction = FUNC_LESS,
                                                  StencilOperation _frontStencilFail = STENCIL_OP_REPLACE,
                                                  StencilOperation _frontStencilPassDepthFail = STENCIL_OP_REPLACE,
                                                  StencilOperation _frontStencilPassDepthPass = STENCIL_OP_REPLACE,
                                                  ComparisonFunction _frontStencilComparisonFunction = FUNC_LESS,
                                                  StencilOperation _backStencilFail = STENCIL_OP_REPLACE,
                                                  StencilOperation _backStencilPassDepthFail = STENCIL_OP_REPLACE,
                                                  StencilOperation _backStencilPassDepthPass = STENCIL_OP_REPLACE,
                                                  ComparisonFunction _backStencilComparisonFunction = FUNC_LESS,
                                                  unsigned int _stencilMask = 0xFF);
    
    extern ResourceHandle CreateSamplerState(TextureFilteringMode _minFilter,
                                             TextureFilteringMode _magFilter,
                                             TextureWrapMode _wrapModeU,
                                             TextureWrapMode _wrapModeV,
                                             TextureWrapMode _wrapModeW,
                                             float _maxAnisotropy = 1.0f,
                                             float _borderRed = 0.0f,
                                             float _borderGreen = 0.0f,
                                             float _borderBlue = 0.0f,
                                             float _borderAlpha = 0.0f);
    
    extern void SetDepthStencilState(ResourceHandle _depthStencilState);
    
    extern void SetRasterizerState(ResourceHandle _rasterizerState);
    
    extern void SetViewport(int _width, int _height, int _topLeftX, int _topLeftY);
    
    extern void Draw(int _firstIndex, int _count);
    
    extern void DrawIndexed(int _indexCount);
    
    extern void DrawIndexedBaseVertex(int _indexCount, unsigned int _baseIndex, unsigned int _baseVertex);
    
    extern void ClearRenderTarget(Vector4 _ClearColor, FramebufferClearTarget _Target);
    
    extern void BindSamplerState(ResourceHandle _SamplerState, int _Slot, ShaderType _shaderStage);
    
    extern void UnbindSamplerState(int _Slot);
    
    extern void BindTexture2D(ResourceHandle _Texture2D, ShaderType _shaderStage);
    
    extern void UnbindTexture2D();
    
    extern void BindTextureCube(ResourceHandle _TextureCube);
    
    extern void BindShaderProgram(ResourceHandle _ShaderProgram);
    
    extern void BindVertexArray(ResourceHandle _VertexArray);
    
    extern void UnbindVertexArray();
    
    extern void BindUniformBuffer(ResourceHandle _uniformBuffer, ShaderType _shaderStage, unsigned int _bufferSlot);
    
    extern void BindVertexBuffer(ResourceHandle _VertexBuffer);
    
    extern void BindIndexBuffer(ResourceHandle _IndexBuffer);
    
    extern void UnbindUniformBuffer();
    
    extern void UnbindVertexBuffer();
    
    extern void UnbindIndexBuffer();
    
    extern void BindFramebuffer(ResourceHandle _Framebuffer = USHRT_MAX);
    
    extern void UnbindFramebuffer();
    
    extern void* MapUniformBuffer(ResourceHandle _UniformBuffer, BufferMapType _Type);
    
    extern void UnmapUniformBuffer();
    
    // Destory Methods - Handles
    
    extern void DestroyVertexBuffer(ResourceHandle _Handle);
    
    extern void DestroyIndexBuffer(ResourceHandle _Handle);
    
    extern void DestroyUniformBuffer(ResourceHandle _Handle);
    
    extern void DestroyVertexArray(ResourceHandle _Handle);
    
    extern void DestroyShader(ResourceHandle _Handle);
    
    extern void DestroyShaderProgram(ResourceHandle _Handle);
    
    extern void DestroySamplerState(ResourceHandle _Handle);

}
        
#endif
