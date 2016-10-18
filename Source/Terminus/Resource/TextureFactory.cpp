#include "TextureFactory.h"
#include "../Graphics/RenderBackend.h"

TextureFactory::TextureFactory()
{

}

TextureFactory::~TextureFactory()
{

}

Texture2D* TextureFactory::Create(asset_common::ImageLoadData* _data)
{
	return RenderBackend::CreateTexture2D(_data->width, _data->height, _data->bytes, true);
}