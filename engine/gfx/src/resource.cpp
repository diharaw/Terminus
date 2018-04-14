#include "resource.hpp"
#include <unordered_map>

PROTOGFX_BEGIN_NAMESPACE

Resource::Resource()
{

}

Resource::~Resource()
{

}

bool Resource::loaded()
{
	return _loaded;
}

ResourceType Resource::type()
{
	return _type;
}

void Resource::set_type(ResourceType type)
{
	_type = type;
}

void Resource::set_status(bool loaded)
{
	_loaded = loaded;
}

namespace resource_manager
{
	using ResourceMap = std::unordered_map<const char*, Resource*>;

	std::unordered_map<ResourceType, ResourceMap> g_resource_map;

	void init()
	{

		g_resource_map[ResourceType::TEXTURE_2D] = ResourceMap();
		g_resource_map[ResourceType::TEXTURE_CUBE] = ResourceMap();
		g_resource_map[ResourceType::MESH] = ResourceMap();
		g_resource_map[ResourceType::IMAGE] = ResourceMap();
		g_resource_map[ResourceType::SHADER] = ResourceMap();
		g_resource_map[ResourceType::SHADER_PROGRAM] = ResourceMap();
		g_resource_map[ResourceType::RENDER_PIPELINE] = ResourceMap();
		g_resource_map[ResourceType::RENDER_PASS] = ResourceMap();
	}

	Resource* load_texture_2d(const char* file)
	{
		ResourceMap map = g_resource_map[ResourceType::TEXTURE_2D];

		if (map.find(file) == map.end())
		{
			return nullptr;
		}
		else
		{
			return nullptr;
		}
	}

	Resource* load_texture_cube(const char** file)
	{
		return nullptr;
	}

	Resource* load_mesh(const char* file)
	{
		return nullptr;
	}

	Resource* load_image(const char* file)
	{
		return nullptr;
	}

	Resource* load_shader(const char* file)
	{
		return nullptr;
	}

	Resource* load_shader_program(const char* file)
	{
		return nullptr;
	}

	Resource* load_render_pipeline(const char* file)
	{
		return nullptr;
	}

	Resource* load_render_pass(const char* file)
	{
		return nullptr;
	}

	void unload_resource(Resource* res)
	{
		
	}

	void clean_up()
	{

	}
}

PROTOGFX_END_NAMESPACE