#pragma once

#ifndef ASSIMPMESHLOADER_H
#define ASSIMPMESHLOADER_H

#include "AssetLoader.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AssimpMeshLoader : public IAssetLoader
{
public:
	AssimpMeshLoader();
	~AssimpMeshLoader();
	void* Load(std::string _id);

private:
	const char* GetTexturePath(aiMaterial* a_Material, aiTextureType a_TextureType);
	bool DoesMaterialExist(std::vector<unsigned int> &_Materials, unsigned int &_CurrentMaterial);
};

#endif