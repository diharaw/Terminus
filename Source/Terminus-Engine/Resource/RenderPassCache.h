#pragma once

#ifndef  RENDERPASSCACHE_H
#define RENDERPASSCACHE_H

#include "AssetCache.h"
#include "RenderPassFactory.h"

namespace terminus
{
	class RenderPassCache : public IAssetCache<RenderPassFactory>
	{
	private:
		std::unordered_map<std::string, RenderPass*> m_RenderPassMap;
		RenderDevice* m_device;

	public:
		RenderPassCache();
		~RenderPassCache();
		void Initialize(RenderDevice* device);
		RenderPass* Load(String key);
		void Unload(Graphics::RenderPass* renderPass);
	};
}

#endif
