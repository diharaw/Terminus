#pragma once

#ifndef ASSIMPMESHLOADER_H
#define ASSIMPMESHLOADER_H

#include <resource/asset_common.h>
#include <core/types.h>

#include <vector>

namespace terminus
{
	namespace assimp_mesh_loader
	{
		extern asset_common::AssimpMeshLoadData* load(String id);
	};
}

#endif
