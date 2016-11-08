#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "AssetCommon.h"
#include "../Graphics/RenderDevice.h"

namespace Terminus { namespace Resource {

	class TextureFactory
	{
	private:
		Graphics::RenderDevice* m_device;

	public:
		TextureFactory();
		~TextureFactory();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::Texture* Create(asset_common::ImageLoadData* _data);
	};

} }

#endif
