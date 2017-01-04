#pragma once

#include "../Core/Config.h"

#if defined(TERMINUS_OPENGL)
#include "render_device_gl.h"
#elif defined(TERMINUS_DIRECT3D11)
#include "render_device_d3d11.h"
#endif
