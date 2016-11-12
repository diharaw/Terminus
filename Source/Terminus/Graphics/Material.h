#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "RenderDevice.h"

// Material Key Options

// Normal Mapping
// Parallax Occlusion
// Diffuse Maps or Constant color
// Roughness Maps or value
// Metalness Maps or value

namespace Terminus { namespace Graphics {

	enum TexureMap
	{
		DIFFUSE = 0,
		NORMAL = 1,
		ROUGHNESS = 2,
		METALNESS = 3,
		DISPLACEMENT = 5,
	};

	struct Material
	{
		Texture2D* texture_maps[5];
		bool 	   backface_cull;
		uint64 	   material_key;
		Vector4    manual_diffuse;
		float 	   manual_roughness;
		float 	   manual_metalness;
	};

} }

#endif