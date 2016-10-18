#pragma once

#ifndef ASSETFACTORY_H
#define ASSETFACTORY_H

#include "../Memory/Allocator.h"

// Forward Declaration
struct IAsset;

class IAssetFactory
{
public:
    IAssetFactory() {}
    virtual ~IAssetFactory() {}
    virtual IAsset* create(void* _load_data, Terminus::Allocator* _allocator = nullptr) = 0;
};

#endif