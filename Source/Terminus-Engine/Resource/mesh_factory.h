#pragma once

#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <Resource/mesh.h>
#include <Graphics/render_device.h>
#include <global.h>
#include <string>

namespace terminus
{
	namespace mesh_factory
	{
		extern Mesh* create(String mesh_name);
	};
}

#endif
