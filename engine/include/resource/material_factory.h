#pragma once

#include <graphics/material.h>
#include <core/types.h>

namespace terminus
{
	namespace material_factory
	{
		extern Material* create(String material_name);
	};
}
