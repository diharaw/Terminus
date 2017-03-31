#pragma once

#include <graphics/render_common.h>
#include <resource/texture_factory.h>
#include <core/global.h>

#include <unordered_map>

namespace terminus
{
	class TextureCache
	{
	private:
		std::unordered_map<std::string, Texture*> m_AssetMap;

	public:
		TextureCache();
		~TextureCache();
		Texture* load(std::string id);
        TextureCube* load_cubemap(std::string id);
		void unload(Texture* texture);
	};
}
