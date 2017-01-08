#include <Resource/stb_image_loader.h>
#include <Resource/asset_common.h>
#include <IO/filesystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace terminus
{
	StbImageLoader::StbImageLoader()
	{
		REGISTER_EXTENSION("png");
		REGISTER_EXTENSION("bmp");
		REGISTER_EXTENSION("jpg");
		REGISTER_EXTENSION("tga");
	}

	StbImageLoader::~StbImageLoader()
	{

	}

	void* StbImageLoader::Load(std::string _id)
	{
		FileHandle handle = filesystem::read_file(_id);

		if (handle.buffer)
		{
			AssetCommon::ImageLoadData* load_data = new AssetCommon::ImageLoadData();

			load_data->bytes = stbi_load_from_memory((const unsigned char*)handle.buffer,
				(int)handle.size,
				&load_data->width,
				&load_data->height,
				&load_data->bpp, 4);

			if (!load_data->bytes)
			{
				std::cout << stbi_failure_reason() << std::endl;
				return nullptr;
			}

			free(handle.buffer);
			return static_cast<void*>(load_data);
		}

		return nullptr;
	}
}
