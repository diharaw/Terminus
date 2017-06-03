#pragma once 

#include <core/macro.h>
#include <graphics/draw_item.h>
#include <graphics/renderer/scene_view.h>

#define MAX_STATIC_MESH_DRAW_ITEMS 1024

TERMINUS_BEGIN_NAMESPACE

struct RenderSubPass;
struct SceneRenderState;
struct SceneView;

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();
	void render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view);

private:
	DrawItem m_draw_items[MAX_VIEWS][MAX_STATIC_MESH_DRAW_ITEMS];
	uint32_t m_num_draw_items[MAX_VIEWS];
};

TERMINUS_END_NAMESPACE