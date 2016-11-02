#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "../Graphics/RenderCommon.h"
#include "AssetCache.h"
#include "TextureFactory.h"

class TextureCache : public IAssetCache<TextureFactory>
{
private:
	std::unordered_map<std::string, ResourceHandle> m_AssetMap;

public:
	TextureCache();
	~TextureCache();

	ResourceHandle Load(std::string _ID);
	void Unload(ResourceHandle _Handle);
};

#endif
