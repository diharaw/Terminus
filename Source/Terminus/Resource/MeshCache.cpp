#include "MeshCache.h"
#include "../IO/FileSystem.h"
#include <iostream>

namespace Terminus { namespace Resource {

	MeshCache::MeshCache()
	{

	}

	MeshCache::~MeshCache()
	{
		for (auto it : m_LoaderList)
		{
			T_SAFE_DELETE(it);
		}
	}

	void MeshCache::Initialize(Graphics::RenderDevice* device, MaterialCache* materialCache)
	{
		m_device = device;
		m_material_cache = materialCache;
		m_Factory.Initialize(m_device);
	}

	Mesh* MeshCache::Load(std::string _ID)
	{
		if (m_AssetMap.find(_ID) == m_AssetMap.end())
		{
			std::cout << "Asset not in cache, loading" << std::endl;

			std::string extension = FileSystem::get_file_extention(_ID);

			if (m_LoaderMap.find(extension) == m_LoaderMap.end())
			{
				std::cout << "No loader found" << std::endl;
				return nullptr;
			}
			else
			{
				AssetCommon::MeshLoadData* data = static_cast<AssetCommon::MeshLoadData*>(m_LoaderMap[extension]->Load(_ID));

				Mesh* mesh = m_Factory.Create(data);
				mesh->SubMeshes = new SubMesh[data->header.m_MeshCount];

				for (int i = 0; i < data->header.m_MeshCount; i++)
				{
					mesh->SubMeshes[i].m_BaseIndex = data->meshes[i].m_BaseIndex;
					mesh->SubMeshes[i].m_BaseVertex = data->meshes[i].m_BaseVertex;
					mesh->SubMeshes[i].m_IndexCount = data->meshes[i].m_IndexCount;
				}

				for (int i = 0; i < data->header.m_MaterialCount; i++)
				{
					String mat_path = std::string(data->materials[i].material);
					std::cout <<  mat_path << std::endl;
				}

				mesh->MeshCount = data->header.m_MeshCount;
				mesh->id = _ID;
				m_AssetMap[_ID] = mesh;

				// Will have to keep vertices and indices around until Physics shape is generated.
				if(data->skeletalVertices)
					free(data->skeletalVertices);

				if (data->meshes)
					free(data->meshes);

				if (data->vertices)
					free(data->vertices);

				if (data->materials)
					free(data->materials);

				free(data);

				std::cout << "Asset successfully loaded" << std::endl;

				return mesh;
			}
		}
		else
		{
			std::cout << "Asset already in cache, returning reference." << std::endl;
			return m_AssetMap[_ID];
		}
	}

	void MeshCache::Unload(Mesh* mesh)
	{
		if (m_AssetMap.find(mesh->id) != m_AssetMap.end())
		{
			m_device->DestroyVertexArray(mesh->VertexArray);
			T_SAFE_DELETE_ARRAY(mesh->SubMeshes);
			T_SAFE_DELETE(mesh);
		}
	}

} }