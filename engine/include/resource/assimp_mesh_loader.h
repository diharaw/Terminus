#pragma once

#ifndef ASSIMPMESHLOADER_H
#define ASSIMPMESHLOADER_H

#include <resource/asset_common.h>
#include <core/types.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

namespace terminus
{
	namespace assimp_mesh_loader
	{
		extern asset_common::AssimpMeshLoadData* load(String id);
	};
}

#endif
