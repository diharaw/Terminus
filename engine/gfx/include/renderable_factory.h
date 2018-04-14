#pragma once

#include <stdint.h>
#include "macros.hpp"

PROTOGFX_BEGIN_NAMESPACE

struct Renderable
{

};

struct TerrainRenderable : public Renderable
{

};

struct MeshRenderable : public Renderable
{

};

struct SkyboxRenderable : public Renderable
{

};

namespace renderable_factory
{
	extern TerrainRenderable* create_terrain(uint32_t width, uint32_t height, const char* height_map);
	extern MeshRenderable* create_mesh(const char* mesh);
	extern SkyboxRenderable* create_skybox(const char* skybox);
}

PROTOGFX_END_NAMESPACE
