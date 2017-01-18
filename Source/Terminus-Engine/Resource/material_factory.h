#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include <Graphics/material.h>
#include <string>

namespace terminus
{
	namespace material_factory
	{
		extern Material* create(asset_common::TextLoadData* data);
	};
}

#endif
