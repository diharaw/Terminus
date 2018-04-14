#include "renderable_factory.h"

PROTOGFX_BEGIN_NAMESPACE

namespace renderable_factory
{
	TerrainRenderable* create_terrain(uint32_t width, uint32_t height, const char* height_map)
	{
		return nullptr;
	}

	MeshRenderable* create_mesh(const char* mesh)
	{
		return nullptr;
	}

	SkyboxRenderable* create_skybox(const char* skybox)
	{
		return nullptr;
	}
}

PROTOGFX_END_NAMESPACE