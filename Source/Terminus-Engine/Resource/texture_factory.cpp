#include <Resource/texture_factory.h>
#include <Core/context.h>

namespace terminus
{

	TextureFactory::TextureFactory()
	{
        
	}

	TextureFactory::~TextureFactory()
	{

	}

	void TextureFactory::Initialize()
	{

	}

	Texture* TextureFactory::Create(AssetCommon::ImageLoadData* _data)
	{
        Task task;
        
        TextureGPUResourceCreateTask* data = task_data<TextureGPUResourceCreateTask>(task);
        
        Texture2D* texture = new Texture2D();
        
        data->data = _data->bytes;
        data->width = _data->width;
        data->height = _data->height;
        
        task._function.Bind<TextureFactory, &TextureFactory::CreateGPUResourcesTask>(this);
        Global::get_context()._rendering_thread.enqueue_upload_task(task);
        
        return texture;
	}

    TASK_METHOD_DEFINITION(TextureFactory, CreateGPUResourcesTask)
    {
        TextureGPUResourceCreateTask* task_data = (TextureGPUResourceCreateTask*)data;
        context::get_render_device().CreateTexture2D(task_data->existing_texture,
                                                     task_data->width,
                                                     task_data->height,
                                                     task_data->data,
                                                     TextureFormat::R8G8B8A8_UNORM,
                                                     false);
    }
    
}
