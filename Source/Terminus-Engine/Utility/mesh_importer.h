#pragma once

#ifndef MESHIMPORTER_H
#define MESHIMPORTER_H

#include <Resource/asset_common.h>
#include <Resource/assimp_mesh_loader.h>
#include <types.h>
#include <Graphics/material.h>
#include <IO/filesystem.h>
#include "prettywriter.h"

namespace terminus
{
    namespace Utility
    {

		inline void import_mesh(String filename)
		{
			String extension = filesystem::get_file_extention(filename);
			String meshPath = filesystem::get_file_path(filename);
	
	        asset_common::AssimpMeshLoadData* data = assimp_mesh_loader::load(filename);
	        
	        std::string fileName = filesystem::get_filename(filename);
	        std::string outputPath = "assets/mesh/";
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
	                String name = filesystem::get_file_name_and_extention(map);
	                String ext = filesystem::get_file_extention(map);
	                std::string texPath = "assets/texture/";
	                texPath += name;
	                
	                String texturePath = "texture/";
	                texturePath += map;
	                
	                rapidjson::Value val(texturePath.c_str(), doc.GetAllocator());
	                doc.AddMember("diffuse_map", val, doc.GetAllocator());
	                
	                String sourcePath = meshPath;
	                sourcePath += map;
	                
	                filesystem::copy_file(sourcePath, texPath);
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
	                String name = filesystem::get_file_name_and_extention(map);
	                String ext = filesystem::get_file_extention(map);
	                std::string texPath = "assets/texture/";
	                texPath += name;
	                
	                String texturePath = "texture/";
	                texturePath += map;
	                
	                rapidjson::Value val(texturePath.c_str(), doc.GetAllocator());
	                doc.AddMember("normal_map", val, doc.GetAllocator());
	                
	                String sourcePath = meshPath;
	                sourcePath += map;
	                
	                filesystem::copy_file(sourcePath, texPath);
	            }
	            
	            // Roughness Value
	            doc.AddMember("roughness_value", 0.5f, doc.GetAllocator());
	            
	            // Metalness Value
	            doc.AddMember("metalness_value", 0.5f, doc.GetAllocator());
	            
	            // Backface Cull
	            doc.AddMember("backface_cull", true, doc.GetAllocator());
	            
	            
	            std::string matPath = "assets/material/mat_";
	            matPath += data->materials[i].m_MeshName;
	            matPath += ".json";
	            
	            doc.Accept(writer);
	            String test = std::string(buffer.GetString());
	            
	            if (filesystem::write_begin(matPath))
	            {
	                filesystem::write((void*)buffer.GetString(), buffer.GetSize(), 1, 0);
	                filesystem::write_end();
	            }
	            
	            String formattedString = "material/mat_";
	            formattedString += data->materials[i].m_MeshName;
	            formattedString += ".json\0";
	            strncpy(mats[i].material, formattedString.c_str(),50);
	        }
	        
	        if (filesystem::write_begin(outputPath))
	        {
	            long Offset = 0;
	            // Write Header
	            filesystem::write(&data->header, sizeof(TSM_FileHeader), 1, Offset);
	            Offset += sizeof(TSM_FileHeader);
	            // Write Vertices
	            filesystem::write(data->vertices, sizeof(Vertex), data->header.m_VertexCount, Offset);
	            Offset += sizeof(Vertex) * data->header.m_VertexCount;
	            // Write Indices
	            filesystem::write(data->indices, sizeof(uint), data->header.m_IndexCount, Offset);
	            Offset += sizeof(uint) * data->header.m_IndexCount;
	            // Write Mesh Headers
	            filesystem::write(data->meshes, sizeof(TSM_MeshHeader), data->header.m_MeshCount, Offset);
	            Offset += sizeof(TSM_MeshHeader) * data->header.m_MeshCount;
	            // Write Materials
	            filesystem::write(mats, sizeof(TSM_Material_Final), data->header.m_MaterialCount, Offset);
	            
	            filesystem::write_end();
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

#endif
