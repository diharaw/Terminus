#pragma once

#include <graphics/render_common.h>
#include <core/global.h>

#include <unordered_map>

namespace terminus
{
    struct PipelineStateObject;
    
    class PipelineStateObjectCache
    {
    private:
        std::unordered_map<std::string, PipelineStateObject*> m_AssetMap;
        
    public:
        PipelineStateObjectCache();
        ~PipelineStateObjectCache();
        PipelineStateObject* load(std::string id);
        void unload(PipelineStateObject* pso);
    };
}
