#include "renderer.hpp"

PROTOGFX_BEGIN_NAMESPACE

namespace renderer
{
	void init(const char* scene)
	{

	}

	TerrainRenderable* create_terrain(uint32_t width, uint32_t height, const char* height_map)
	{
		return nullptr;
	}

	MeshRenderable*	create_mesh(const char* mesh)
	{
		return nullptr;
	}

	SkyboxRenderable* create_skybox(const char* skybox)
	{
		return nullptr;
	}

	PointLight*	create_point_light()
	{
		return nullptr;
	}

	DirectionalLight* create_direcitonal_light()
	{
		return nullptr;
	}
	SpotLight* create_spot_light()
	{
		return nullptr;
	}

	void shutdown()
	{

	}
}

PROTOGFX_END_NAMESPACE