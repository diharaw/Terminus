#pragma once

#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include "../Resource/AssetCommon.h"
#include "../Resource/AssimpMeshLoader.h"
#include "../Types.h"
#include "../Graphics/Material.h"
#include "../IO/FileSystem.h"
#include "prettywriter.h"

namespace Terminus { namespace Utility {

	

	inline void ImportMesh(String filename)
	{
		Resource::AssimpMeshLoader loader;
		
		String extension = FileSystem::get_file_extention(filename);
		String meshPath = FileSystem::get_file_path(filename);

		for (String ext : loader.m_Extensions)
		{
			if (extension == ext)
			{
				AssetCommon::AssimpMeshLoadData* data = (AssetCommon::AssimpMeshLoadData*)loader.Load(filename);

				std::string fileName = FileSystem::get_filename(filename);
				std::string outputPath = "Assets/Mesh/";
				outputPath += fileName;
				outputPath += ".tsm";

				TSM_Material_Final* mats = new TSM_Material_Final[data->header.m_MaterialCount];

				for (int i = 0; i < data->header.m_MaterialCount; i++)
				{
					JsonDocument doc;
					doc.SetObject();
					rapidjson::StringBuffer buffer;
					rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
					
					// Diffuse
					String map = std::string(data->materials[i].m_Albedo);

					if (map != "")
					{
						String name = FileSystem::get_file_name_and_extention(map);
						String ext = FileSystem::get_file_extention(map);
						std::string texPath = "Assets/Texture/";
						texPath += name;

						String texturePath = "Texture/";
						texturePath += map;

						rapidjson::Value val(texturePath.c_str(), doc.GetAllocator());
						doc.AddMember("diffuse_map", val, doc.GetAllocator());

						String sourcePath = meshPath;
						sourcePath += map;

						FileSystem::copy_file(sourcePath, texPath);
					}
					else
					{
						rapidjson::Value val(rapidjson::kObjectType);
						
						val.AddMember("r", 0.0f, doc.GetAllocator());
						val.AddMember("b", 0.0f, doc.GetAllocator());
						val.AddMember("g", 0.0f, doc.GetAllocator());
						val.AddMember("a", 1.0f, doc.GetAllocator());

						doc.AddMember("diffuse_value", val, doc.GetAllocator());
					}
				
					// Normal
					map = std::string(data->materials[i].m_Normal);

					if (map != "")
					{
						String name = FileSystem::get_file_name_and_extention(map);
						String ext = FileSystem::get_file_extention(map);
						std::string texPath = "Assets/Texture/";
						texPath += name;

						String texturePath = "Texture/";
						texturePath += map;

						rapidjson::Value val(texturePath.c_str(), doc.GetAllocator());
						doc.AddMember("normal_map", val, doc.GetAllocator());

						String sourcePath = meshPath;
						sourcePath += map;

						FileSystem::copy_file(sourcePath, texPath);
					}

					// Roughness Value
					doc.AddMember("roughness_value", 0.5f, doc.GetAllocator());

					// Metalness Value
					doc.AddMember("metalness_value", 0.5f, doc.GetAllocator());

					// Backface Cull
					doc.AddMember("backface_cull", true, doc.GetAllocator());

					
					std::string matPath = "Assets/Material/Mat_";
					matPath += data->materials[i].m_MeshName;
					matPath += ".json";

					doc.Accept(writer);
					String test = std::string(buffer.GetString());

					if (FileSystem::write_begin(matPath))
					{
						FileSystem::write((void*)buffer.GetString(), buffer.GetSize(), 1, 0);
						FileSystem::write_end();
					}

					strncpy(&mats[i].material[0], matPath.c_str(), matPath.size());
				}
				
				if (FileSystem::write_begin(outputPath))
				{
					long Offset = 0;
					// Write Header
					FileSystem::write(&data->header, sizeof(TSM_FileHeader), 1, Offset);
					Offset += sizeof(TSM_FileHeader);
					// Write Vertices
					FileSystem::write(data->vertices, sizeof(Vertex), data->header.m_VertexCount, Offset);
					Offset += sizeof(Vertex) * data->header.m_VertexCount;
					// Write Indices
					FileSystem::write(data->indices, sizeof(uint), data->header.m_IndexCount, Offset);
					Offset += sizeof(uint) * data->header.m_IndexCount;
					// Write Mesh Headers
					FileSystem::write(data->meshes, sizeof(TSM_MeshHeader), data->header.m_MeshCount, Offset);
					Offset += sizeof(TSM_MeshHeader) * data->header.m_MeshCount;
					// Write Materials
					FileSystem::write(&mats, sizeof(TSM_Material_Final), data->header.m_MaterialCount, Offset);

					FileSystem::write_end();
				}

				T_SAFE_DELETE_ARRAY(mats);
				T_SAFE_DELETE_ARRAY(data->vertices);
				T_SAFE_DELETE_ARRAY(data->indices);
				T_SAFE_DELETE_ARRAY(data->materials);
				T_SAFE_DELETE_ARRAY(data->meshes);
				T_SAFE_DELETE(data);
			}
		}
	}

} }

#endif