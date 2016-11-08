#include "TextureFactory.h"

namespace Terminus { namespace Resource {

	TextureFactory::TextureFactory()
	{

	}

	TextureFactory::~TextureFactory()
	{

	}

	void TextureFactory::Initialize(Terminus::Graphics::RenderDevice* device)
	{
		m_device = device;
	}

	Terminus::Graphics::Texture* TextureFactory::Create(asset_common::ImageLoadData* _data)
	{
		return m_device->CreateTexture2D(_data->width, _data->height, _data->bytes, TextureFormat::R8G8B8A8_UNORM);
	}

} }