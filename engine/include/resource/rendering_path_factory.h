#pragma once

#include <graphics/rendering_path.h>
#include <core/types.h>

namespace terminus
{
    namespace rendering_path_factory
    {
        extern RenderingPath* create(String rendering_path);
    }
}
