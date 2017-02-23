#pragma once

#include <ECS/scene.h>
#include <Resource/asset_common.h>
#include <Resource/mesh_cache.h>
#include <editor/editor_scene.h>

namespace terminus
{
    namespace scene_factory
    {
        Scene* create(asset_common::TextLoadData* _data);
        Scene* create(EditorScene* editor_scene);
        EditorScene* create_editor_scene(asset_common::TextLoadData* _data);
    };
} // namespace terminus
