#pragma once

#ifndef RENDERPASSCACHE_H
#define RENDERPASSCACHE_H

#include <Resource/render_pass_factory.h>

namespace terminus
{
	class RenderPassCache
	{
	private:
		std::unordered_map<std::string, RenderPass*> m_RenderPassMap;

	public:
		RenderPassCache();
		~RenderPassCache();
		void initialize();
		RenderPass* load(String key);
		void unload(RenderPass* renderPass);
	};
}

#endif
