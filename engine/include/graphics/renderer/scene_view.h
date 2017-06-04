#pragma once

#include <graphics/renderer/renderable.h>
#include <graphics/renderer/renderer_common.h>

#define MAX_VIEWS 10

TERMINUS_BEGIN_NAMESPACE

struct RenderingPath;
struct CommandBuffer;
struct CommandPool;
struct LinearAllocator;

struct SceneView
{
	Entity		     camera_entity;
	uint16_t	     z_index;
	uint16_t	     scene_index;
	Vector4		     screen_rect;
	Vector4		     clear_color;
	Texture*	     render_target;
	bool		     is_shadow;
	RenderingPath*   rendering_path;
	CommandPool*     cmd_pool;
	uint16_t	     num_cmd_buffers;
	CommandBuffer*   cmd_buffers[4];
	LinearAllocator* allocator;
	PerFrameUniforms per_frame_uniforms;
};

TERMINUS_END_NAMESPACE
