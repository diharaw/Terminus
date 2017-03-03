#pragma once

#include <Core/config.h>
#include <imgui.h>

#if defined(TERMINUS_OPENGL)
#include <Graphics/graphics_types_gl.h>
#elif defined(TERMINUS_DIRECT3D11)
#include <Graphics/graphics_types_d3d11.h>
#endif

namespace ImGui
{
    extern void ImageWithTexture(terminus::Texture* texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
}
