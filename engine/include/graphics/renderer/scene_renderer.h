#pragma once 

#include <core/macro.h>
#include <graphics/renderer/mesh_renderer.h>

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	void render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view);

private:
	MeshRenderer m_mesh_renderer;
};

TERMINUS_END_NAMESPACE