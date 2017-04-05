#include <resource/texture_cache.h>
#include <resource/stb_image_loader.h>
#include <io/filesystem.h>
#include <core/context.h>
#include <core/types.h>

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
        device.destroy_texture_2d((Texture2D*)task_data->_texture);
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
    
    TextureCube* TextureCache::load_cubemap(std::string id)
    {
        if (m_AssetMap.find(id) == m_AssetMap.end())
        {
            std::cout << "Asset not in Cache. Loading Asset." << std::endl;
            
            TextureCube* texture = texture_factory::create_cubemap(id);
            m_AssetMap[id] = texture;
            
            std::cout << "Asset successfully loaded" << std::endl;
            
            return texture;
        }
        else
        {
            std::cout << "Asset already loaded. Returning reference." << std::endl;
            return (TextureCube*)m_AssetMap[id];
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
