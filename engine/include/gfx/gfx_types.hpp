#pragma once

#if defined(TE_GFX_BACKEND_GL) || defined(TE_GFX_BACKEND_GLES)
#include <gfx/opengl/gfx_types_gl.hpp>
#elif defined(TE_GFX_BACKEND_D3D11)
#include <gfx/direct3d11/gfx_types_d3d11.hpp>
#elif defined(TE_GFX_BACKEND_VK)
#include <gfx/vulkan/gfx_types_vk.hpp>
#endif