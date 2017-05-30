#pragma once

#include <core/macro.h>
#include <core/types.h>
#include <gameplay/entity.h>
#include <graphics/draw_item.h>

#define MAX_STATIC_RENDERABLES 1024
#define MAX_SKELETAL_RENDERABLES 512
#define MAX_TERRAINS 10
#define MAX_BONES 100

TERMINUS_BEGIN_NAMESPACE

struct Mesh;

struct StaticRenderable
{
	Entity		   entity;
	Mesh*          mesh;
	bool           sub_mesh_cull;
	float          radius;
	Matrix4 	   transform;
	Vector3 	   position;
	RenderableType type;
};

struct SkeletalRenderable
{
	Entity		   entity;
	Mesh*          mesh;
	bool           sub_mesh_cull;
	float          radius;
	Matrix4 	   transform;
	Matrix4		   bones[MAX_BONES];
	Vector3 	   position;
	RenderableType type;
};

struct TerrainRenderable
{

};

TERMINUS_END_NAMESPACE
