#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include <Graphics/material.h>
#include <types.h>

namespace terminus
{
	namespace material_factory
	{
		extern Material* create(String material_name);
	};
}

#endif
