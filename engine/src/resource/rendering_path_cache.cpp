#include <resource/rendering_path_cache.h>
#include <resource/rendering_path_factory.h>
#include <core/context.h>

namespace terminus
{
    RenderingPathCache::RenderingPathCache()
    {
        filesystem::add_directory("assets/rendering_path");
    }
    
    RenderingPathCache::~RenderingPathCache()
    {
        
    }
    
    RenderingPath* RenderingPathCache::load(String rendering_path)
    {
        if(_rendering_path_map.find(rendering_path) == _rendering_path_map.end())
        {
            RenderingPath* rp = rendering_path_factory::create(rendering_path);
            _rendering_path_map[rendering_path] = rp;
            return rp;
        }
        return _rendering_path_map[rendering_path];
    }
    
    void RenderingPathCache::unload(RenderingPath* rendering_path)
    {
        RenderPassCache& cache = context::get_render_pass_cache();
        
        for(int i = 0; i < rendering_path->_num_render_passes; i++)
        {
            cache.unload(rendering_path->_render_passes[i]);
        }
        
        T_SAFE_DELETE(rendering_path);
    }
}
