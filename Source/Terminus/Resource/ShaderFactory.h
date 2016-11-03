#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "AssetCommon.h"

class ShaderFactory
{
public:
    ShaderFactory();
    ~ShaderFactory();
    
    ResourceHandle Create(asset_common::TextLoadData* _data);
};

#endif
