#pragma once

#include <core/macro.h>
#include <container/packed_array.h>
#include <gameplay/entity.h>
#include <graphics/renderer/scene_view.h>
#include <gameplay/entity_data_map.h>

#define MAX_DRAW_ITEMS 1024

TERMINUS_BEGIN_NAMESPACE

// Forward Declarations

class  Scene;
struct Entity;
struct FramePacket;
struct SkyComponent;

// Type Definitions

using DrawItemArray   = std::array<DrawItem, MAX_DRAW_ITEMS>;
using SceneViewArray  = PackedArray<SceneView, MAX_VIEWS>;

class RenderSystem
{
private:
	EntityDataMap<StaticRenderable, MAX_ENTITIES> m_static_renderables;
	EntityDataMap<SceneView, MAX_VIEWS>			  m_scene_views;
	Scene*										  m_scene;
    
public:
    DrawItem      m_skydome_item;
    Mesh*         m_skydome_mesh;
    SkyComponent* m_sky_cmp;
    
public:
    RenderSystem();
    ~RenderSystem();
    void initialize(Scene* scene);
    void simulate(FramePacket* pkt, double delta);
    void shutdown();
    void on_entity_created(Entity entity);
    void on_entity_destroyed(Entity entity);
};

TERMINUS_END_NAMESPACE
