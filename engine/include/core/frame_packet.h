#pragma once

#include <stdint.h>
#include <graphics/renderer/scene_view.h>

TERMINUS_BEGIN_NAMESPACE

struct CommandPool;
struct CommandBuffer;

#define MAX_COMMAND_POOLS 10

struct SceneRenderState
{
	uint32_t		   num_renderables;
	StaticRenderable   static_renderables[MAX_STATIC_RENDERABLES];
	SkeletalRenderable skeletal_renderables[MAX_SKELETAL_RENDERABLES];
	uint16_t		   view_count;
	CommandPool*	   cmd_pool[4];
	CommandBuffer*	   cmd_buffers[4];
};

struct FramePacket
{
	uint32_t		 cmd_buffer_count;
	CommandBuffer*	 cmd_buffers[32];
	uint16_t		 scene_count;
	SceneRenderState scene_render_states[10];
	uint16_t		 total_views;
	SceneView		 views[MAX_VIEWS];
};

TERMINUS_END_NAMESPACE