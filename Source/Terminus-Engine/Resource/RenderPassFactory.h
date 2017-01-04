#pragma once

#ifndef  RENDERPASSFACTORY_H
#define RENDERPASSFACTORY_H

#include "../Graphics/RenderPass.h"
#include "../Graphics/render_device.h"

namespace terminus
{
	class RenderPassFactory
	{
	private:
		RenderDevice* m_device;

	public:
		RenderPassFactory();
		~RenderPassFactory();
		void Initialize(RenderDevice* device);

		RenderPass* Create(AssetCommon::TextLoadData* _data);
	};
}

#endif
