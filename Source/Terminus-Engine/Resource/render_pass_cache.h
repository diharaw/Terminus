#pragma once

#ifndef  RENDERPASSCACHE_H
#define RENDERPASSCACHE_H

#include <Resource/asset_cache.h>
#include <Resource/render_pass_factory.h>

namespace terminus
{
	class RenderPassCache : public IAssetCache<RenderPassFactory>
	{
	private:
		std::unordered_map<std::string, RenderPass*> m_RenderPassMap;

	public:
		RenderPassCache();
		~RenderPassCache();
		void Initialize();
		RenderPass* Load(String key);
		void Unload(RenderPass* renderPass);
	};
}

#endif
