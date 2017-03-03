#include <Resource/texture_cache.h>
#include <Resource/stb_image_loader.h>
#include <IO/filesystem.h>
#include <Core/context.h>
#include <types.h>

#include <iostream>

namespace terminus
{
    struct TextureUnloadTaskData
    {
        Texture* _texture;
    };
    
    void texture_unload_task(void* data)
    {
        TextureUnloadTaskData* task_data = (TextureUnloadTaskData*)data;
        RenderDevice& device = context::get_render_device();
        device.DestroyTexture2D((Texture2D*)task_data->_texture);
    }
    
	TextureCache::TextureCache()
	{
        filesystem::add_directory("assets/texture");
	}

	TextureCache::~TextureCache()
	{

	}

	Texture* TextureCache::load(std::string id)
	{
		if (m_AssetMap.find(id) == m_AssetMap.end())
		{
            std::cout << "Asset not in Cache. Loading Asset." << std::endl;

            Texture* texture = texture_factory::create(id);
            m_AssetMap[id] = texture;

            std::cout << "Asset successfully loaded" << std::endl;

            return texture;
        }
		else
		{
			std::cout << "Asset already loaded. Returning reference." << std::endl;
			return m_AssetMap[id];
		}
	}

	void TextureCache::unload(Texture* texture)
	{
        Task task;
        TextureUnloadTaskData* data = task_data<TextureUnloadTaskData>(task);
        data->_texture = texture;
        task._function.Bind<&texture_unload_task>();
        submit_gpu_upload_task(task);
	}
}
