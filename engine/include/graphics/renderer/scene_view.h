#pragma once

#include <graphics/renderer/renderable.h>

#define MAX_VIEWS 10

TERMINUS_BEGIN_NAMESPACE

struct RenderingPath;

struct SceneView
{
	Entity			 camera_entity;
	uint16_t		 z_index;
	Matrix4			 view_matrix;
	Matrix4			 projection_matrix;
	Matrix4			 view_projection_matrix;
	Vector4			 screen_rect;
	Texture*		 render_target;
	bool			 is_shadow;
	RenderingPath*	 rendering_path;
};

TERMINUS_END_NAMESPACE
