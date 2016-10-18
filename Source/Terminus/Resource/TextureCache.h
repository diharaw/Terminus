#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "../Graphics/RenderCommon.h"
#include "AssetCache.h"
#include "TextureFactory.h"

class TextureCache : public IAssetCache<TextureFactory>
{
private:
	std::unordered_map<std::string, Texture2D*> m_AssetMap;

public:
	TextureCache();
	~TextureCache();

	Texture2D* Load(std::string _ID);
	void Unload(Texture2D* _Handle);
};

#endif
