#include "ShaderCache.h"
#include "../IO/FileSystem.h"
#include "../Graphics/RenderBackend.h"

ShaderCache::ShaderCache()
{
    
}

ShaderCache::~ShaderCache()
{
    
}

ResourceHandle ShaderCache::Load(const char* _vertexID,
                                 const char* _pixelID,
                                 const char* _geometryID,
                                 const char* _tessevalID,
                                 const char* _tesscontrolID)
{
    ResourceHandle vertex, pixel, geometry, tess_eval, tess_control;
    
    // load vertex shader
    {
        if(_vertexID != nullptr)
        {
            std::string id = std::string(_vertexID);
            
            if (m_ShaderMap.find(id) == m_ShaderMap.end())
            {
                std::string extension = FileSystem::get_file_extention(id);
                
                if (m_LoaderMap.find(extension) == m_LoaderMap.end())
                {
                    return USHRT_MAX;
                }
                else
                {
                    asset_common::TextLoadData* data = static_cast<asset_common::TextLoadData*>(m_LoaderMap[extension]->Load(id));
                    data->shader_type = SHADER_VERTEX;
                    
                    vertex = m_Factory.Create(data);
                }
            }
            else
                vertex = m_ShaderMap[id];
        }
        else
            return USHRT_MAX;
    }
    
    // load pixel shader
    {
        if(_vertexID != nullptr)
        {
            std::string id = std::string(_pixelID);
            
            if (m_ShaderMap.find(id) == m_ShaderMap.end())
            {
                std::string extension = FileSystem::get_file_extention(id);
                
                if (m_LoaderMap.find(extension) == m_LoaderMap.end())
                {
                    return USHRT_MAX;
                }
                else
                {
                    asset_common::TextLoadData* data = static_cast<asset_common::TextLoadData*>(m_LoaderMap[extension]->Load(id));
                    data->shader_type = SHADER_PIXEL;
                    
                    pixel = m_Factory.Create(data);
                }
            }
            else
                pixel = m_ShaderMap[id];
        }
        else
            return USHRT_MAX;
    }
    
    // load geometry shader
    {
        if(_geometryID != nullptr)
        {
            std::string id = std::string(_geometryID);
            
            if (m_ShaderMap.find(id) == m_ShaderMap.end())
            {
                std::string extension = FileSystem::get_file_extention(id);
                
                if (m_LoaderMap.find(extension) == m_LoaderMap.end())
                {
                    return USHRT_MAX;
                }
                else
                {
                    asset_common::TextLoadData* data = static_cast<asset_common::TextLoadData*>(m_LoaderMap[extension]->Load(id));
                    data->shader_type = SHADER_GEOMETRY;
                    
                    geometry = m_Factory.Create(data);
                }
            }
            else
                geometry = m_ShaderMap[id];
        }
        else
            return USHRT_MAX;
    }
    
    // load tessellation evaluation shader
    {
        if(_tessevalID != nullptr)
        {
            std::string id = std::string(_tessevalID);
            
            if (m_ShaderMap.find(id) == m_ShaderMap.end())
            {
                std::string extension = FileSystem::get_file_extention(id);
                
                if (m_LoaderMap.find(extension) == m_LoaderMap.end())
                {
                    return USHRT_MAX;
                }
                else
                {
                    asset_common::TextLoadData* data = static_cast<asset_common::TextLoadData*>(m_LoaderMap[extension]->Load(id));
                    data->shader_type = SHADER_TESSELLATION_EVALUATION;
                    
                    tess_eval = m_Factory.Create(data);
                }
            }
            else
                tess_eval = m_ShaderMap[id];
        }
        else
            return USHRT_MAX;
    }
    
    // load tessellation control shader
    {
        if(_tesscontrolID != nullptr)
        {
            std::string id = std::string(_tesscontrolID);
            
            if (m_ShaderMap.find(id) == m_ShaderMap.end())
            {
                std::string extension = FileSystem::get_file_extention(id);
                
                if (m_LoaderMap.find(extension) == m_LoaderMap.end())
                {
                    return USHRT_MAX;
                }
                else
                {
                    asset_common::TextLoadData* data = static_cast<asset_common::TextLoadData*>(m_LoaderMap[extension]->Load(id));
                    data->shader_type = SHADER_TESSELLATION_CONTROL;
                    
                    tess_control = m_Factory.Create(data);
                }
            }
            else
                tess_control = m_ShaderMap[id];
        }
        else
            return USHRT_MAX;
    }
    
    ResourceHandle program = RenderBackend::CreateShaderProgram(vertex, geometry, tess_control, tess_eval, pixel);
    
    if(HANDLE_VALID(program))
    {
        m_ShaderProgramMap[vertex] = program;
        m_ShaderProgramMap[pixel] = program;
        
        if(HANDLE_VALID(geometry))
            m_ShaderProgramMap[geometry] = program;
        if(HANDLE_VALID(tess_control))
            m_ShaderProgramMap[tess_control] = program;
        if(HANDLE_VALID(tess_eval))
            m_ShaderProgramMap[tess_eval] = program;
        
        return program;
    }
    else
        return USHRT_MAX;
}

void ShaderCache::Unload(ResourceHandle _Handle)
{
    
}
