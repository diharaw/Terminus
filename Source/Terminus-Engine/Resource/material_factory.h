#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include <Graphics/material.h>
#include <Resource/texture_cache.h>
#include <string>

namespace terminus
{
	class MaterialFactory
	{
	public:
		MaterialFactory();
		~MaterialFactory();
		Material* Create(AssetCommon::TextLoadData* data);
	};
}

#endif
