#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <Graphics/render_common.h>
#include <Resource/texture_factory.h>
#include <global.h>

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
		void unload(Texture* texture);
	};
}

#endif
