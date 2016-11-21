#pragma once

#ifndef  RENDERPASSCACHE_H
#define RENDERPASSCACHE_H

#include "AssetCache.h"
#include "RenderPassFactory.h"

namespace Terminus { namespace Resource {

	class RenderPassCache : public IAssetCache<RenderPassFactory>
	{
	private:
		std::unordered_map<std::string, Graphics::RenderPass*> m_RenderPassMap;
		Graphics::RenderDevice* m_device;

	public:
		RenderPassCache();
		~RenderPassCache();
		void Initialize(Graphics::RenderDevice* device);
		Graphics::RenderPass* Load(String key);
		void Unload(Graphics::RenderPass* renderPass);
	};

} }

#endif