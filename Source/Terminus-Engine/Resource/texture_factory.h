#pragma once

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include <Graphics/render_common.h>
#include <Graphics/render_device.h>
#include <Resource/asset_common.h>
#include <global.h>

namespace terminus
{
    struct CreateTextureTaskData
    {
        asset_common::ImageLoadData* load_data;
        Texture2D*                   existing_texture;
    };
    
	namespace texture_factory
	{
		extern Texture* create(asset_common::ImageLoadData* data);
        extern TASK_METHOD_DECLARATION(create_texture_task);
	};
}

#endif
