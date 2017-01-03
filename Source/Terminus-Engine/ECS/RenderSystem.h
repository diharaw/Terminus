#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "../Global.h"
#include "../Graphics/RenderingPath.h"
#include "../Graphics/DrawItem.h"
#include "../Graphics/Renderer.h"
#include "../Resource/ShaderCache.h"

#define MAX_DRAW_ITEMS 1024
#define MAX_VIEWS 10
#define MAX_RENDERABLES 1024

namespace Terminus { namespace ECS {
    
    // Forward Declarations
    
    struct SceneView;
    struct Renderable;
    
    // Type Definitions
    
    using DrawItemArray   = std::array<Graphics::DrawItem, MAX_DRAW_ITEMS>;
    using SceneViewArray  = std::array<SceneView, MAX_VIEWS>;
    using RenderableArray = std::array<Renderable, MAX_RENDERABLES>;
    
    struct RenderPrepareTaskData
    {
        int _scene_index;
    };
    
    struct Renderable
    {
        Mesh*                    _mesh;
        bool                     _sub_mesh_cull;
        float                    _radius;
        TransformComponent*      _transform;
        Graphics::RenderableType _type;
        // TODO : Accomodate material overrides.
        // TODO : Union containing Renderable type (Mesh, Ocean, Terrain etc)
    };
    
    struct SceneView
    {
        DrawItemArray            _draw_items;
        int                      _num_items;
        Matrix4*                 _view_matrix;
        Matrix4*                 _projection_matrix;
        Matrix4*                 _view_projection_matrix;
        Vector4                  _screen_rect;
        bool                     _is_shadow;
        uint32                   _cmd_buf_idx;
        Graphics::RenderingPath* _rendering_path;
    };
    
    class RenderSystem : public ISystem
    {
    private:
        SceneViewArray          m_views;
        uint16                  m_view_count;
        RenderableArray         m_renderables;
        uint16                  m_renderable_count;
        ThreadPool*             m_thread_pool;
        Graphics::Renderer*     m_renderer;
        Resource::ShaderCache*  m_shader_cache;
        
    public:
        RenderSystem();
        ~RenderSystem();
        void SetRenderDevice(Graphics::Renderer* renderer);
        void SetShaderCache(Resource::ShaderCache* shaderCache);
        virtual void Initialize();
        virtual void Update(double delta);
        virtual void Shutdown();
        void OnEntityCreated(Entity entity);
        void OnEntityDestroyed(Entity entity);
        
    private:
        TASK_METHOD_DECLARATION(RenderPrepareTask);
    };
    
} }

#endif
