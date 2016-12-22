#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "AssetCommon.h"
#include "../Graphics/RenderDevice.h"
#include "../Global.h"

namespace Terminus { namespace Resource {

    struct TextureGPUResourceCreateTask
    {
        uint16 width;
        uint16 height;
        void* data;
    };
    
	class TextureFactory
	{
	private:
		Graphics::RenderDevice* m_device;
        Graphics::Texture* m_texture;
        ThreadPool* m_rendering_thread_pool;
        TextureGPUResourceCreateTask m_task_data;

	public:
		TextureFactory();
		~TextureFactory();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::Texture* Create(AssetCommon::ImageLoadData* _data);
        
    private:
        TASK_METHOD_DECLARATION(CreateGPUResourcesTask);
	};

} }

#endif
