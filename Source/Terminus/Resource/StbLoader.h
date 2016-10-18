#pragma once

#ifndef STBLOADER_H
#define STBLOADER_H

#include "AssetLoader.h"

class StbLoader : public IAssetLoader
{
public:
    StbLoader();
    ~StbLoader();
    void* Load(std::string _id);
};

#endif