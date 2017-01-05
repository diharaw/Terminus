#pragma once

#ifndef  RENDERPASSFACTORY_H
#define RENDERPASSFACTORY_H

#include "../Graphics/RenderPass.h"
#include "../Graphics/render_device.h"

namespace terminus
{
	class RenderPassFactory
	{
	public:
		RenderPassFactory();
		~RenderPassFactory();
		void Initialize();

		RenderPass* Create(AssetCommon::TextLoadData* _data);
	};
}

#endif
