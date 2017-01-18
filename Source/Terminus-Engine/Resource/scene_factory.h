#pragma once

#include <ECS/scene.h>
#include <Resource/asset_common.h>
#include <Resource/mesh_cache.h>

namespace terminus
{
    namespace scene_factory
    {
        Scene* create(asset_common::TextLoadData* _data);
    };
} // namespace terminus
