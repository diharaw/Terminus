#pragma once

#include "macros.hpp"

PROTOGFX_BEGIN_NAMESPACE

enum class ResourceType
{
	TEXTURE_2D = 0,
	TEXTURE_CUBE = 1,
	MESH = 2,
	IMAGE = 3,
	SHADER = 4,
	SHADER_PROGRAM = 5,
	RENDER_PIPELINE = 6,
	RENDER_PASS = 6,
};

class Resource
{
public:
	Resource();
	~Resource();
	bool loaded();
	ResourceType type();
	void set_type(ResourceType type);
	void set_status(bool loaded);

private:
	ResourceType _type;
	bool _loaded;
};

namespace resource_manager
{
	extern void init();
	extern Resource* load_texture_2d(const char* file);
	extern Resource* load_texture_cube(const char** file);
	extern Resource* load_mesh(const char* file);
	extern Resource* load_image(const char* file);
	extern Resource* load_shader(const char* file);
	extern Resource* load_shader_program(const char* file);
	extern Resource* load_render_pipeline(const char* file);
	extern Resource* load_render_pass(const char* file);
	extern void unload_resource(Resource* res);
	extern void clean_up();
};

PROTOGFX_END_NAMESPACE
