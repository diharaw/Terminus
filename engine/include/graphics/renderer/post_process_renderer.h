#pragma once 

#include <core/macro.h>

TERMINUS_BEGIN_NAMESPACE

struct RenderSubPass;
struct SceneRenderState;
struct SceneView;

class PostProcessRenderer
{
public:
	PostProcessRenderer();
	~PostProcessRenderer();
	void render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view);
};

TERMINUS_END_NAMESPACE