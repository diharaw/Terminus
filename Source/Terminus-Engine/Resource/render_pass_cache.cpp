#include <Resource/render_pass_cache.h>
#include <IO/filesystem.h>

#include <iostream>

namespace terminus
{
	RenderPassCache::RenderPassCache()
	{
        _last_id = 0;
	}

	RenderPassCache::~RenderPassCache()
	{

	}

	void RenderPassCache::initialize()
	{
		
	}

	RenderPass* RenderPassCache::load(String key)
	{
		if (_render_pass_map.find(key) == _render_pass_map.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
            
            RenderPass* render_pass = render_pass_factory::create(key);
            render_pass->pass_id = _last_id++;
            _render_pass_map[key] = render_pass;
            return render_pass;
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return _render_pass_map[key];
		}
	}

	void RenderPassCache::unload(RenderPass* renderPass)
	{
		T_SAFE_DELETE(renderPass);
	}

} // namespace terminus
