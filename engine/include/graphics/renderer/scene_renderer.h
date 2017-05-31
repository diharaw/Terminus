#pragma once 

#include <core/macro.h>
#include <graphics/renderer/mesh_renderer.h>

TERMINUS_BEGIN_NAMESPACE

struct FramePacket;
struct RenderPass;

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer();
	void render(RenderPass* render_pass, FramePacket* pkt);

private:
	MeshRenderer m_mesh_renderer;
};

TERMINUS_END_NAMESPACE