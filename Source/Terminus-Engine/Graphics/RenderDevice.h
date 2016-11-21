#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include "Config.h"

#if defined(TERMINUS_OPENGL)
#include "GLRenderDevice.h"
#elif defined(TERMINUS_DIRECT3D11)
#include "D3D11RenderDevice.h"
#endif

#endif
