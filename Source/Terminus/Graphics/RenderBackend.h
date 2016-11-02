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
	extern ResourceHandle CreateFramebuffer(Texture2D* _RenderTargets, int _Count, Texture2D* _DepthTarget);
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
	extern ResourceHandle CreateShaderProgram(Shader* _Vertex,
											  Shader* _Geometry,
											  Shader* _TessellationControl,
											  Shader* _TessellationEvalution,
											  Shader* _Pixel);
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
	extern ResourceHandle CreateVertexArray(ResourceHandle _vertexBuffer, ResourceHandle _indexBuffer, InputLayoutType _layoutType = LAYOUT_STANDARD_VERTEX, InputLayout* _layout = nullptr);
}

#endif
