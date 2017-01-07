#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "AssetCommon.h"
#include "../Graphics/render_device.h"
#include "../Global.h"

namespace terminus
{
    struct TextureGPUResourceCreateTask
    {
        uint16 width;
        uint16 height;
        void* data;
    };
    
	class TextureFactory
	{
	private:
        Texture* m_texture;
        RenderingThreadPool* m_rendering_thread_pool;
        TextureGPUResourceCreateTask m_task_data;

	public:
		TextureFactory();
		~TextureFactory();
		void Initialize();

		Texture* Create(AssetCommon::ImageLoadData* _data);
        
    private:
        TASK_METHOD_DECLARATION(CreateGPUResourcesTask);
	};
}

#endif
