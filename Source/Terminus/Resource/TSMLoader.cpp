#include "TSMLoader.h"
#include "AssetCommon.h"
#include "../IO/FileSystem.h"

TSMLoader::TSMLoader()
{
	REGISTER_EXTENSION("tsm");
}

TSMLoader::~TSMLoader()
{

}

void* TSMLoader::Load(std::string _id)
{
	FileHandle handle = FileSystem::ReadFile(_id);

	if (handle.buffer)
	{
		asset_common::MeshLoadData* load_data = new asset_common::MeshLoadData();

		size_t offset = 0;;

		memcpy(&load_data->header, handle.buffer, sizeof(TSM_FileHeader));

		load_data->vertices = new Vertex[load_data->header.m_VertexCount];
		load_data->indices = new uint[load_data->header.m_IndexCount];
		load_data->meshes = new TSM_MeshHeader[load_data->header.m_MeshCount];

		offset += sizeof(TSM_FileHeader);

		memcpy(load_data->vertices, handle.buffer + offset, sizeof(Vertex) * load_data->header.m_VertexCount);
		offset += sizeof(Vertex) * load_data->header.m_VertexCount;

		memcpy(load_data->indices, handle.buffer + offset, sizeof(uint) * load_data->header.m_IndexCount);
		offset += sizeof(uint) * load_data->header.m_IndexCount;

		memcpy(load_data->meshes, handle.buffer + offset, sizeof(TSM_MeshHeader) * load_data->header.m_MeshCount);

		return static_cast<void*>(load_data);
	}
	
	return nullptr;
}