#pragma once

#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include "AssetLoader.h"

namespace Terminus { namespace Resource {

	class TextLoader : public IAssetLoader
	{
	public:
		TextLoader();
		~TextLoader();
		void* Load(std::string _id);
	};

} }

#endif
