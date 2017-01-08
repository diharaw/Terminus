#pragma once

#ifndef ASSETFACTORY_H
#define ASSETFACTORY_H

#include <Memory/allocator.h>

// Forward Declaration
struct IAsset;

class IAssetFactory
{
public:
    IAssetFactory() {}
    virtual ~IAssetFactory() {}
    virtual IAsset* create(void* _load_data, terminus::Allocator* _allocator = nullptr) = 0;
};

#endif
