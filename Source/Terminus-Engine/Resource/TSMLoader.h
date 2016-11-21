#pragma once

#ifndef TSMLOADER_H
#define TSMLOADER_H

#include "AssetLoader.h"

namespace Terminus { namespace Resource {

	class TSMLoader : public IAssetLoader
	{
	public:
		TSMLoader();
		~TSMLoader();
		void* Load(std::string _id);
	};

} }

#endif
