#include <resource/assimp_mesh_loader.h>
#include <resource/asset_common.h>
#include <io/filesystem.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace terminus
{
	namespace assimp_mesh_loader
    {
        const char* get_texture_path(aiMaterial* a_Material, aiTextureType a_TextureType)
        {
            aiString path;
            aiReturn result = a_Material->GetTexture(a_TextureType, 0, &path);
            
            if (result == aiReturn_FAILURE)
            {
                return "";
            }
            else
            {
                String cppStr = std::string(path.C_Str());
                
                if (cppStr == "")
                    return nullptr;
                
                const char* cPath = path.C_Str();
                return cPath;
            }
        }
        
        bool does_material_exist(std::vector<unsigned int> &_Materials, unsigned int &_CurrentMaterial)
        {
            for (auto it : _Materials)
            {
                if (it == _CurrentMaterial)
                    return true;
            }
            
            return false;
        }

        asset_common::AssimpMeshLoadData* load(String id)
        {
            {
                asset_common::AssimpMeshLoadData* load_data = new asset_common::AssimpMeshLoadData();
                const aiScene* Scene;
                Assimp::Importer Importer;
                Scene = Importer.ReadFile(id.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
                
                load_data->header.m_MeshCount = Scene->mNumMeshes;
                load_data->header.m_VertexCount = 0;
                load_data->header.m_IndexCount = 0;
                load_data->header.m_MaterialCount = Scene->mNumMaterials;
                load_data->meshes = new TSM_MeshHeader[Scene->mNumMeshes];
                
                // Temp
                load_data->IsSkeletal = false;
                
                aiMaterial* TempMaterial;
                uint8 materialIndex = 0;
                
                std::vector<Assimp_Material> temp_materials;
                std::vector<unsigned int> processedMatId;
                
                for (int i = 0; i < load_data->header.m_MeshCount; i++)
                {
                    load_data->meshes[i].m_IndexCount = Scene->mMeshes[i]->mNumFaces * 3;
                    load_data->meshes[i].m_BaseIndex = load_data->header.m_IndexCount;
                    load_data->meshes[i].m_BaseVertex = load_data->header.m_VertexCount;
                    
                    load_data->header.m_VertexCount += Scene->mMeshes[i]->mNumVertices;
                    load_data->header.m_IndexCount += load_data->meshes[i].m_IndexCount;
                    
                    std::cout << Scene->mMeshes[i]->mMaterialIndex << std::endl;
                    
                    if (!does_material_exist(processedMatId, Scene->mMeshes[i]->mMaterialIndex))
                    {
                        processedMatId.push_back(Scene->mMeshes[i]->mMaterialIndex);
                        load_data->meshes[i].m_MaterialIndex = materialIndex;
                        
                        Assimp_Material temp;
                        
                        TempMaterial = Scene->mMaterials[Scene->mMeshes[i]->mMaterialIndex];
                        temp.m_MeshName = std::string(Scene->mMeshes[i]->mName.C_Str());
                        
                        const char* albedo = get_texture_path(TempMaterial, aiTextureType_DIFFUSE);
                        
                        if(albedo != nullptr)
                            strncpy(temp.m_Albedo, albedo, 50);
                        
                        const char* normal = get_texture_path(TempMaterial, aiTextureType_HEIGHT);
                        
                        if (normal != nullptr)
                            strncpy(temp.m_Normal, normal, 50);
                        
                        temp_materials.push_back(temp);
                        materialIndex++;
                    }
                }
                
                load_data->header.m_MaterialCount = processedMatId.size();
                
                if (load_data->header.m_MaterialCount > 0)
                    load_data->materials = new Assimp_Material[load_data->header.m_MaterialCount];
                else
                    load_data->materials = nullptr;
                
                for (int i = 0; i < temp_materials.size(); i++)
                {
                    load_data->materials[i] = temp_materials[i];
                }
                
                load_data->vertices = new Vertex[load_data->header.m_VertexCount];
                load_data->indices = new uint[load_data->header.m_IndexCount];
                
                aiMesh* TempMesh;
                int idx = 0;
                int vertexIndex = 0;
                
                for (int i = 0; i < load_data->header.m_MeshCount; i++)
                {
                    TempMesh = Scene->mMeshes[i];
                    load_data->meshes[i].m_MaxExtents = Vector3(TempMesh->mVertices[0].x, TempMesh->mVertices[0].y, TempMesh->mVertices[0].z);
                    load_data->meshes[i].m_MinExtents = Vector3(TempMesh->mVertices[0].x, TempMesh->mVertices[0].y, TempMesh->mVertices[0].z);
                    
                    for (int k = 0; k < Scene->mMeshes[i]->mNumVertices; k++)
                    {
                        load_data->vertices[vertexIndex].m_Position = glm::vec3(TempMesh->mVertices[k].x, TempMesh->mVertices[k].y, TempMesh->mVertices[k].z);
                        load_data->vertices[vertexIndex].m_Normal = glm::vec3(TempMesh->mNormals[k].x, TempMesh->mNormals[k].y, TempMesh->mNormals[k].z);
                        
                        if (load_data->vertices[vertexIndex].m_Position.x > load_data->meshes[i].m_MaxExtents.x)
                            load_data->meshes[i].m_MaxExtents.x = load_data->vertices[vertexIndex].m_Position.x;
                        if (load_data->vertices[vertexIndex].m_Position.y > load_data->meshes[i].m_MaxExtents.y)
                            load_data->meshes[i].m_MaxExtents.y = load_data->vertices[vertexIndex].m_Position.y;
                        if (load_data->vertices[vertexIndex].m_Position.z > load_data->meshes[i].m_MaxExtents.z)
                            load_data->meshes[i].m_MaxExtents.z = load_data->vertices[vertexIndex].m_Position.z;
                        
                        if (load_data->vertices[vertexIndex].m_Position.x < load_data->meshes[i].m_MinExtents.x)
                            load_data->meshes[i].m_MinExtents.x = load_data->vertices[vertexIndex].m_Position.x;
                        if (load_data->vertices[vertexIndex].m_Position.y < load_data->meshes[i].m_MinExtents.y)
                            load_data->meshes[i].m_MinExtents.y = load_data->vertices[vertexIndex].m_Position.y;
                        if (load_data->vertices[vertexIndex].m_Position.z < load_data->meshes[i].m_MinExtents.z)
                            load_data->meshes[i].m_MinExtents.z = load_data->vertices[vertexIndex].m_Position.z;
                        
                        if (TempMesh->HasTextureCoords(0))
                        {
                            load_data->vertices[vertexIndex].m_TexCoord = glm::vec2(TempMesh->mTextureCoords[0][k].x, TempMesh->mTextureCoords[0][k].y);
                        }
                        
                        vertexIndex++;
                    }
                    
                    for (int j = 0; j < TempMesh->mNumFaces; j++)
                    {
                        load_data->indices[idx] = TempMesh->mFaces[j].mIndices[0];
                        idx++;
                        load_data->indices[idx] = TempMesh->mFaces[j].mIndices[1];
                        idx++;
                        load_data->indices[idx] = TempMesh->mFaces[j].mIndices[2];
                        idx++;
                    }
                }
                
                load_data->header.m_MaxExtents = load_data->meshes[0].m_MaxExtents;
                load_data->header.m_MinExtents = load_data->meshes[0].m_MinExtents;
                
                for (int i = 0; i < load_data->header.m_MeshCount; i++)
                {
                    if (load_data->meshes[i].m_MaxExtents.x > load_data->header.m_MaxExtents.x)
                        load_data->header.m_MaxExtents.x = load_data->meshes[i].m_MaxExtents.x;
                    if (load_data->meshes[i].m_MaxExtents.y > load_data->header.m_MaxExtents.y)
                        load_data->header.m_MaxExtents.y = load_data->meshes[i].m_MaxExtents.y;
                    if (load_data->meshes[i].m_MaxExtents.z > load_data->header.m_MaxExtents.z)
                        load_data->header.m_MaxExtents.z = load_data->meshes[i].m_MaxExtents.z;
                    
                    if (load_data->meshes[i].m_MinExtents.x < load_data->header.m_MinExtents.x)
                        load_data->header.m_MinExtents.x = load_data->meshes[i].m_MinExtents.x;
                    if (load_data->meshes[i].m_MinExtents.y < load_data->header.m_MinExtents.y)
                        load_data->header.m_MinExtents.y = load_data->meshes[i].m_MinExtents.y;
                    if (load_data->meshes[i].m_MinExtents.z < load_data->header.m_MinExtents.z)
                        load_data->header.m_MinExtents.z = load_data->meshes[i].m_MinExtents.z;
                }
                
                return load_data;
            }
            
            return nullptr;
        }
    }
} // namespace terminus
