#pragma once 

#include <core/macro.h>
#include <graphics/draw_item.h>
#include <graphics/renderer/scene_view.h>

#define MAX_STATIC_MESH_DRAW_ITEMS 1024

TERMINUS_BEGIN_NAMESPACE

struct RenderSubPass;
struct SceneRenderState;
struct SceneView;
struct UniformBuffer;

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();
	void render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view);

private:
	DrawItem m_draw_items[MAX_VIEWS][MAX_STATIC_MESH_DRAW_ITEMS];
	uint32_t m_num_draw_items[MAX_VIEWS];
	UniformBuffer* m_per_frame_buf;
	UniformBuffer* m_per_draw_buf;
	UniformBuffer* m_per_material_buf;
};

TERMINUS_END_NAMESPACE