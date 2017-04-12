#include <resource/pipeline_state_object_cache.h>
#include <resource/pipeline_state_object_factory.h>
#include <io/filesystem.h>

namespace terminus
{
    PipelineStateObjectCache::PipelineStateObjectCache()
    {
        filesystem::add_directory("assets/pipeline_state_object");
    }
    
    PipelineStateObjectCache::~PipelineStateObjectCache()
    {
        
    }
    
    PipelineStateObject* PipelineStateObjectCache::load(std::string id)
    {
        if (m_AssetMap.find(id) == m_AssetMap.end())
        {
            T_LOG_INFO("Asset not in Cache. Loading Asset.");
            
            PipelineStateObject* pso = pipeline_state_object_factory::create(id);
            m_AssetMap[id] = pso;
            
            T_LOG_INFO("Asset successfully loaded");
            
            return pso;
        }
        else
        {
            T_LOG_INFO("Asset already loaded. Returning reference.");
            return m_AssetMap[id];
        }
    }
    
    void PipelineStateObjectCache::unload(PipelineStateObject* pso)
    {
        
    }
}
