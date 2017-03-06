#pragma once

#include <core/config.h>

#if defined(TERMINUS_OPENGL)
#include <graphics/render_device_gl.h>
#elif defined(TERMINUS_DIRECT3D11)
#include <graphics/render_device_d3d11.h>
#endif
