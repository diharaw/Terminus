#pragma once

#include <graphics/renderer/renderable.h>

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
	Matrix4		     view_matrix;
	Matrix4		     projection_matrix;
	Matrix4		     view_projection_matrix;
	Vector3		     camera_pos;
	Vector4		     screen_rect;
	Texture*	     render_target;
	bool		     is_shadow;
	RenderingPath*   rendering_path;
	CommandPool*     cmd_pool;
	uint16_t	     num_cmd_buffers;
	CommandBuffer*   cmd_buffers[4];
	LinearAllocator* allocator;
};

TERMINUS_END_NAMESPACE
