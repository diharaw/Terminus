#include <Resource/stb_image_loader.h>
#include <Resource/asset_common.h>
#include <IO/filesystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace terminus
{
	namespace stb_image_loader
    {
        asset_common::ImageLoadData* load(std::string id)
        {
            FileHandle handle = filesystem::read_file(id);
            
            if (handle.buffer)
            {
                asset_common::ImageLoadData* load_data = new asset_common::ImageLoadData();
                
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
                return load_data;
            }
            
            return nullptr;
        }
    }
}
