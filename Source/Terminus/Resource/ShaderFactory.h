#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "../Graphics/RenderDevice.h"
#include "AssetCommon.h"

namespace Terminus { namespace Resource {

	class ShaderFactory
	{
	private:
		Graphics::RenderDevice* m_device;

	public:
		ShaderFactory();
		~ShaderFactory();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::Shader* Create(AssetCommon::TextLoadData* _data);
	};

} }

#endif
