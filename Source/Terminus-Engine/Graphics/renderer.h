#pragma once

#include "render_device.h"
#include "GraphicsQueue.h"

namespace terminus
{
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
        bool          _front_queue_index;
        GraphicsQueue _graphics_queues[2];
        
    public:
        UniformBuffer* _per_frame_buffer;
        UniformBuffer* _per_draw_buffer;
        UniformBuffer* _per_draw_material_buffer;
        UniformBuffer* _per_draw_bone_offsets_buffer;
        
    public:
        Renderer();
        ~Renderer();
        void initialize();
        void submit();
        void swap();
        uint32 create_command_buffer();
        CommandBuffer& command_buffer(uint32 index);
        LinearAllocator* uniform_allocator();
        
    private:
        GraphicsQueue& graphics_queue_front();
        GraphicsQueue& graphics_queue_back();
        
	};

}
