#include "StbLoader.h"
#include "AssetCommon.h"
#include "../IO/FileSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

StbLoader::StbLoader()
{
	REGISTER_EXTENSION("png");
	REGISTER_EXTENSION("bmp");
	REGISTER_EXTENSION("jpg");
	REGISTER_EXTENSION("tga");
}

StbLoader::~StbLoader()
{
    
}

void* StbLoader::Load(std::string _id)
{
    FileHandle handle = FileSystem::read_file(_id);
    
    if(handle.buffer)
    {
        asset_common::ImageLoadData* load_data = new asset_common::ImageLoadData();
        
        load_data->bytes = stbi_load_from_memory((const unsigned char*)handle.buffer,
                                                 (int)handle.size,
                                                 &load_data->width,
                                                 &load_data->height,
                                                 &load_data->bpp , 4);
        
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
