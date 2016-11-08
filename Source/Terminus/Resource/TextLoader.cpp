#include "TextLoader.h"
#include "AssetCommon.h"
#include "../IO/FileSystem.h"

namespace Terminus { namespace Resource {

	TextLoader::TextLoader()
	{
		REGISTER_EXTENSION("as");
		REGISTER_EXTENSION("lua");
		REGISTER_EXTENSION("hlsl");
		REGISTER_EXTENSION("glsl");
		REGISTER_EXTENSION("cfg");
		REGISTER_EXTENSION("ini");
		REGISTER_EXTENSION("txt");
	}

	TextLoader::~TextLoader()
	{

	}

	void* TextLoader::Load(std::string _id)
	{
		FileHandle handle = FileSystem::read_file(_id, true);

		if (handle.buffer)
		{
			asset_common::TextLoadData* load_data = new asset_common::TextLoadData();

			load_data->buffer = handle.buffer;

			return static_cast<void*>(load_data);
		}

		return nullptr;
	}

} }