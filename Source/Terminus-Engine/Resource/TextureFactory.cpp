#include "TextureFactory.h"
#include <Core/context.h>

namespace terminus
{

	TextureFactory::TextureFactory()
	{
        m_rendering_thread_pool = Global::GetRenderingThreadPool();
	}

	TextureFactory::~TextureFactory()
	{

	}

	void TextureFactory::Initialize()
	{

	}

	Texture* TextureFactory::Create(AssetCommon::ImageLoadData* _data)
	{
        m_texture = nullptr;
        
        m_task_data.data = _data->bytes;
        m_task_data.width = _data->width;
        m_task_data.height = _data->height;
        
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->data = (void*)&m_task_data;
        task->function.Bind<TextureFactory, &TextureFactory::CreateGPUResourcesTask>(this);
        m_rendering_thread_pool->SubmitAndWait();
        
        if(m_texture)
            return m_texture;
        
        return nullptr;
	}

    TASK_METHOD_DEFINITION(TextureFactory, CreateGPUResourcesTask)
    {
        TextureGPUResourceCreateTask* task_data = (TextureGPUResourceCreateTask*)data;
        m_texture = context::get_render_device().CreateTexture2D(task_data->width, task_data->height, task_data->data, TextureFormat::R8G8B8A8_UNORM, false);
    }
    
}
