#pragma once

#include <resource/mesh.h>
#include <graphics/render_device.h>
#include <core/global.h>
#include <string>

namespace terminus
{
	namespace mesh_factory
	{
		extern Mesh* create(String mesh_name);
	};
}
