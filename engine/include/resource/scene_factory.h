#pragma once

#include <resource/asset_common.h>
#include <resource/mesh_cache.h>

namespace terminus
{
	class Scene;

    namespace scene_factory
    {
        Scene* create(asset_common::TextLoadData* _data);
    };
} // namespace terminus
