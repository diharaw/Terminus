#pragma once

#include <graphics/render_common.h>
#include <graphics/render_device.h>
#include <resource/asset_common.h>
#include <core/global.h>

namespace terminus
{
    namespace pipeline_state_object_factory
    {
        extern PipelineStateObject* create(String pso);
    };
}
