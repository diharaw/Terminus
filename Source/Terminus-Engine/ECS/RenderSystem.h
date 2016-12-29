#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "../Global.h"
#include "../Graphics/RenderingPath.h"

#define MAX_DRAW_ITEMS 1024
#define MAX_VIEWS 10

namespace Terminus { namespace ECS {
    
    struct FrustumCullTaskData
    {
        int start_index;
        int item_count;
    };
    
    enum class DrawItemType
    {
        StaticMesh = 0,
        SkeletalMesh,
        Terrain,
        Ocean,
        Particle,
        Skybox
    };
    
    struct DrawItem
    {
        uint64 sort_key;
        DrawItemType type;
        Mesh* mesh;
        Matrix4* transform;
    };
    
    struct SceneView
    {
        DrawItem visible_draw_items[MAX_DRAW_ITEMS];
        uint16 visible_item_count;
        Matrix4 view_matrix;
        Matrix4 projection_matrix;
        Matrix4 view_projection_matrix;
        Vector4 screen_rect;
        bool is_shadow;
        Graphics::RenderingPath* rendering_path;
    };
    
    class RenderSystem : public ISystem
    {
    private:
        SceneView m_views[MAX_VIEWS];
        uint16 m_view_count;
        
        ThreadPool* m_thread_pool;
        
    public:
        RenderSystem();
        ~RenderSystem();
        virtual void Initialize();
        virtual void Update(double delta);
        virtual void Shutdown();
        void OnEntityCreated(Entity entity);
        void OnEntityDestroyed(Entity entity);
        
    private:
        TASK_METHOD_DECLARATION(FrustumCullTask);
    };
    
} }

#endif
