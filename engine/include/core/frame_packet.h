#pragma once

#include <stdint.h>
#include <graphics/renderer/scene_view.h>

TERMINUS_BEGIN_NAMESPACE

struct CommandPool;
struct CommandBuffer;

struct SceneRenderState
{
	uint32_t		 num_renderables;
	StaticRenderable static_renderables[MAX_RENDERABLES];
	uint16_t		 view_count;
	SceneView		 views[MAX_VIEWS];
};

struct FramePacket
{
	CommandPool*	 cmd_pool;
	uint32_t		 cmd_buffer_count;
	CommandBuffer*	 cmd_buffers;
	uint16_t		 scene_count;
	SceneRenderState scene_render_states[10];
};

TERMINUS_END_NAMESPACE