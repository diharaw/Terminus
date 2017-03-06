#pragma once

#include <graphics/rendering_path.h>
#include <core/types.h>
#include <unordered_map>

namespace terminus
{
    class RenderingPathCache
    {
    public:
        std::unordered_map<String, RenderingPath*> _rendering_path_map;
        
    public:
        RenderingPathCache();
        ~RenderingPathCache();
        RenderingPath* load(String rendering_path);
        void unload(RenderingPath* rendering_path);
    };
}
