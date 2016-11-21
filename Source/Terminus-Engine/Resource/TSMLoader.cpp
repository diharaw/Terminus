#include "TSMLoader.h"
#include "AssetCommon.h"
#include "../IO/FileSystem.h"

namespace Terminus { namespace Resource {

	TSMLoader::TSMLoader()
	{
		REGISTER_EXTENSION("tsm");
	}

	TSMLoader::~TSMLoader()
	{

	}

	void* TSMLoader::Load(std::string _id)
	{
		FileHandle handle = FileSystem::read_file(_id);

		if (handle.buffer)
		{
			AssetCommon::MeshLoadData* load_data = new AssetCommon::MeshLoadData();

			size_t offset = 0;;

			memcpy(&load_data->header, handle.buffer, sizeof(TSM_FileHeader));

			load_data->vertices = new Vertex[load_data->header.m_VertexCount];
			load_data->indices = new uint[load_data->header.m_IndexCount];
			load_data->meshes = new TSM_MeshHeader[load_data->header.m_MeshCount];
			load_data->materials = new TSM_Material_Final[load_data->header.m_MaterialCount];

			offset += sizeof(TSM_FileHeader);

			memcpy(load_data->vertices, handle.buffer + offset, sizeof(Vertex) * load_data->header.m_VertexCount);
			offset += sizeof(Vertex) * load_data->header.m_VertexCount;

			memcpy(load_data->indices, handle.buffer + offset, sizeof(uint) * load_data->header.m_IndexCount);
			offset += sizeof(uint) * load_data->header.m_IndexCount;

			memcpy(load_data->meshes, handle.buffer + offset, sizeof(TSM_MeshHeader) * load_data->header.m_MeshCount);
			offset += sizeof(TSM_MeshHeader) * load_data->header.m_MeshCount;

			memcpy(load_data->materials, handle.buffer + offset, sizeof(TSM_Material_Final) * load_data->header.m_MaterialCount);

			free(handle.buffer);

			return static_cast<void*>(load_data);
		}

		return nullptr;
	}

} }