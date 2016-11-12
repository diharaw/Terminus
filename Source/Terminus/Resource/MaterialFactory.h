#pragma once

#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "../Graphics/Material.h"
#include <string>

namespace Terminus { namespace Resource {

	class MaterialFactory
	{
	private:
		Graphics::RenderDevice* m_device;

	public:
		MaterialFactory();
		~MaterialFactory();
		void Initialize(Graphics::RenderDevice* device);
		Graphics::Material* Create(asset_common::TextLoadData* _Data);
	};

} }

#endif
