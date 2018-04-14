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

struct Light
{

};

struct DirectionalLight : public Light
{

};

struct SpotLight : public Light
{

};

struct PointLight : public Light
{

};

namespace renderer
{
	extern void				  init(const char* scene);
	extern TerrainRenderable* create_terrain(uint32_t width, uint32_t height, const char* height_map);
	extern MeshRenderable*	  create_mesh(const char* mesh);
	extern SkyboxRenderable*  create_skybox(const char* skybox);
	extern PointLight*		  create_point_light();
	extern DirectionalLight*  create_direcitonal_light();
	extern SpotLight*		  create_spot_light();
	extern void				  shutdown();
}

PROTOGFX_END_NAMESPACE
