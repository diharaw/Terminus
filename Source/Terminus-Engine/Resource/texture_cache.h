#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <Graphics/render_common.h>
#include <Resource/asset_cache.h>
#include <Resource/texture_factory.h>
#include <global.h>

namespace terminus
{
	class TextureCache : public IAssetCache<TextureFactory>
	{
	private:
		std::unordered_map<std::string, Texture*> m_AssetMap;

	public:
		TextureCache();
		~TextureCache();
		void Initialize();

		Texture* Load(std::string _ID);
		void Unload(Texture* texture);
	};
}

#endif
