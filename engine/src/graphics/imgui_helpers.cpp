#include <graphics/imgui_helpers.h>

namespace ImGui
{
	void ImageWithTexture(terminus::Texture* texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
	{
	#ifdef TERMINUS_OPENGL
		ImGui::Image((ImTextureID)texture->m_id, size, uv0, uv1, tint_col, border_col);
	#else
		ImGui::Image((ImTextureID)texture->m_textureView, size, uv0, uv1, tint_col, border_col);
	#endif
	}
}
