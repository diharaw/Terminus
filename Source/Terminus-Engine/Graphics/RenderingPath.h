#ifndef RENDERINGPATH_H
#define RENDERINGPATH_H

#include "RenderPass.h"

#define MAX_RENDER_PASSES 10

namespace Terminus { namespace Graphics {

    struct RenderingPath
    {
        String     _name;
        RenderPass _render_passes[MAX_RENDER_PASSES];
    };
    
} }

#endif
