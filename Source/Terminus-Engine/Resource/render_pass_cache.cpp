#include <Resource/render_pass_cache.h>
#include <IO/filesystem.h>

#include <iostream>

namespace terminus
{
	RenderPassCache::RenderPassCache()
	{

	}

	RenderPassCache::~RenderPassCache()
	{

	}

	void RenderPassCache::initialize()
	{
		
	}

	RenderPass* RenderPassCache::load(String key)
	{
		if (m_RenderPassMap.find(key) == m_RenderPassMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
            
            RenderPass* render_pass = render_pass_factory::create(key);
            m_RenderPassMap[key] = render_pass;
            return render_pass;
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return m_RenderPassMap[key];
		}
	}

	void RenderPassCache::unload(RenderPass* renderPass)
	{
		T_SAFE_DELETE(renderPass);
	}

} // namespace terminus
