#pragma once

#ifndef  RENDERPASSFACTORY_H
#define RENDERPASSFACTORY_H

#include "../Graphics/RenderPass.h"
#include "../Graphics/RenderDevice.h"

namespace Terminus { namespace Resource {

	class RenderPassFactory
	{
	private:
		Graphics::RenderDevice* m_device;

	public:
		RenderPassFactory();
		~RenderPassFactory();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::RenderPass* Create(AssetCommon::TextLoadData* _data);
	};

} }

#endif
