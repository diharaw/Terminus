#pragma once

#include "macros.hpp"
#include "gfx_enums.hpp"
#include "gfx_descs.hpp"

#if defined(GFX_BACKEND_GL3)
    #include "gfx_types_gl3.hpp"
#elif defined(GFX_BACKEND_GL4)
    #include "gfx_types_gl4.hpp"
#elif defined(GFX_BACKEND_GLES2)
    #include "gfx_types_gles2.hpp"
#elif defined(GFX_BACKEND_GLES3)
    #include "gfx_types_gles3.hpp"
#elif defined(GFX_BACKEND_D3D11)
    #include "gfx_types_d3d11.hpp"
#elif defined(GFX_BACKEND_D3D12)
    #include "gfx_types_d3d12.hpp"
#elif defined(GFX_BACKEND_VK)
    #include "gfx_types_vk.hpp"
#elif defined(GFX_BACKEND_MTL)
    #include "gfx_types_mtl.hpp"
#endif
