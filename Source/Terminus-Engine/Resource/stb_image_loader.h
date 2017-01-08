#pragma once

#ifndef STBIMAGELOADER_H
#define STBIMAGELOADER_H

#include <Resource/asset_loader.h>

namespace terminus
{
	class StbImageLoader : public IAssetLoader
	{
	public:
		StbImageLoader();
		~StbImageLoader();
		void* Load(std::string _id);
	};
}

#endif
