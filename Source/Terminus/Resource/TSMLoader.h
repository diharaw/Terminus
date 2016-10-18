#pragma once

#ifndef TSMLOADER_H
#define TSMLOADER_H

#include "AssetLoader.h"

class TSMLoader : public IAssetLoader
{
public:
	TSMLoader();
	~TSMLoader();
	void* Load(std::string _id);
};

#endif
