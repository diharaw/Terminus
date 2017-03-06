#pragma once

#include <gameplay/scene.h>
#include <resource/asset_common.h>
#include <resource/mesh_cache.h>

namespace terminus
{
    namespace scene_factory
    {
        Scene* create(asset_common::TextLoadData* _data);
    };
} // namespace terminus
