#pragma once

#include <Graphics/rendering_path.h>
#include <types.h>

namespace terminus
{
    namespace rendering_path_factory
    {
        extern RenderingPath* create(String rendering_path);
    }
}
