#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderDevice.h"
#include "GraphicsQueue.h"

namespace Terminus { namespace Graphics {

    // Uniform Slots
    
#define PER_FRAME_UNIFORM_SLOT 1
#define PER_DRAW_UNIFORM_SLOT 2
#define PER_DRAW_MATERIAL_UNIFORM_SLOT 3
#define PER_DRAW_BONE_OFFSETS_UNIFORM_SLOT 4
    
    // Uniform Buffer Structs
    struct T_ALIGNED(16) PerFrameUniforms
    {
        Matrix4 last_view_projection;
        Matrix4 view_projection;
        Matrix4 inverse_view_projection;
        Matrix4 projection;
        Matrix4 view;
        Vector3 view_position;
        Vector3 view_direction;
    };
    
    struct T_ALIGNED(16) PerDrawUniforms
    {
        Matrix4 model_view_projection;
        Matrix4 model;
        Vector3 position;
    };
    
    struct T_ALIGNED(16) PerDrawMaterialUniforms
    {
        Vector4 diffuse;
        float   roughness;
        float   metalness;
    };
    
    struct T_ALIGNED(16) PerDrawBoneOffsetUniforms
    {
        Matrix4 bone_offset[100];
    };
    
	class Renderer
	{
    private:
        bool          m_front_queue_index;
        GraphicsQueue m_graphics_queues[2];
        
    public:
        Graphics::UniformBuffer* m_per_frame_buffer;
        Graphics::UniformBuffer* m_per_draw_buffer;
        Graphics::UniformBuffer* m_per_draw_material_buffer;
        Graphics::UniformBuffer* m_per_draw_bone_offsets_buffer;
        
    public:
        Renderer();
        ~Renderer();
        void Initialize(RenderDevice& device);
        void Submit(RenderDevice& device);
        void Swap();
        uint32 CreateCommandBuffer();
        CommandBuffer& GetCommandBuffer(uint32 index);
        LinearAllocator* GetUniformAllocator();
        
    private:
        GraphicsQueue& GetGraphicsQueueFront();
        GraphicsQueue& GetGraphicsQueueBack();
        
	};

} }

#endif
