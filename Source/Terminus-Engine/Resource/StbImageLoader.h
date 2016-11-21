#pragma once

#ifndef STBIMAGELOADER_H
#define STBIMAGELOADER_H

#include "AssetLoader.h"

namespace Terminus { namespace Resource {

	class StbImageLoader : public IAssetLoader
	{
	public:
		StbImageLoader();
		~StbImageLoader();
		void* Load(std::string _id);
	};

} }

#endif