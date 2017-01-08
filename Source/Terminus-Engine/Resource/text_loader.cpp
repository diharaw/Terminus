#include <Resource/text_loader.h>
#include <Resource/asset_common.h>
#include <IO/filesystem.h>

namespace terminus
{
	TextLoader::TextLoader()
	{
		REGISTER_EXTENSION("as");
		REGISTER_EXTENSION("lua");
		REGISTER_EXTENSION("hlsl");
		REGISTER_EXTENSION("glsl");
		REGISTER_EXTENSION("cfg");
		REGISTER_EXTENSION("ini");
		REGISTER_EXTENSION("txt");
        REGISTER_EXTENSION("json");
	}

	TextLoader::~TextLoader()
	{

	}

	void* TextLoader::Load(std::string _id)
	{
		FileHandle handle = filesystem::read_file(_id, true);

		if (handle.buffer)
		{
			AssetCommon::TextLoadData* load_data = new AssetCommon::TextLoadData();

			load_data->buffer = handle.buffer;

			return static_cast<void*>(load_data);
		}

		return nullptr;
	}
} // namespace terminus
