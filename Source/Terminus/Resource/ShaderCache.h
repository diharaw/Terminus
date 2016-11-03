#pragma once

#ifndef SHADERCACHE_H
#define SHADERCACHE_H

#include "AssetCache.h"
#include "AssetLoader.h"
#include "ShaderFactory.h"
#include <unordered_map>
#include "../Types.h"

class ShaderCache : public IAssetCache<ShaderFactory>
{
private:
    std::unordered_map<std::string, ResourceHandle> m_ShaderMap;
    std::unordered_map<ResourceHandle, ResourceHandle> m_ShaderProgramMap;
    
public:
    ShaderCache();
    ~ShaderCache();
    
    ResourceHandle Load(const char* _vertexID,
                        const char* _pixelID,
                        const char* _geometryID = nullptr,
                        const char* _tessevalID = nullptr,
                        const char* _tesscontrolID = nullptr);
    void Unload(ResourceHandle _Handle);
};

#endif
