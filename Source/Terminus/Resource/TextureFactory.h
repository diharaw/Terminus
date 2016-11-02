#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "AssetCommon.h"

struct Texture2D;

class TextureFactory
{
public:
	TextureFactory();
	~TextureFactory();

	ResourceHandle Create(asset_common::ImageLoadData* _data);
};

#endif
