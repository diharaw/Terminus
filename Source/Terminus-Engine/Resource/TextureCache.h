#pragma once

#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include "../Graphics/RenderCommon.h"
#include "AssetCache.h"
#include "TextureFactory.h"

namespace Terminus { namespace Resource {

	class TextureCache : public IAssetCache<TextureFactory>
	{
	private:
		std::unordered_map<std::string, Graphics::Texture*> m_AssetMap;
		Graphics::RenderDevice* m_device;

	public:
		TextureCache();
		~TextureCache();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::Texture* Load(std::string _ID);
		void Unload(Graphics::Texture* texture);
	};

} }

#endif
