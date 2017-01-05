#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "../Graphics/RenderCommon.h"
#include "AssetCache.h"
#include "TextureFactory.h"
#include "../Global.h"

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
