#pragma once

#define TE_BACKEND_VULKAN

#if defined(TE_BACKEND_VULKAN)
#include <gfx/vulkan/gfx_device_vk.hpp>
#elif defined(TE_BACKEND_OPENGL)
#include <gfx/opengl/gfx_device_gl.hpp>
#endif