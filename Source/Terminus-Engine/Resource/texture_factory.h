#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include <Graphics/render_common.h>
#include <Graphics/render_device.h>
#include <Resource/asset_common.h>
#include <global.h>

namespace terminus
{
    struct TextureGPUResourceCreateTask
    {
        uint16     width;
        uint16     height;
        void*      data;
        Texture2D* existing_texture;
    };
    
	class TextureFactory
	{
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
