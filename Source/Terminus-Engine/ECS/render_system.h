#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <global.h>
#include <Graphics/rendering_path.h>
#include <Graphics/draw_item.h>
#include <Graphics/renderer.h>
#include <Resource/shader_cache.h>
#include <Resource/mesh.h>

#define MAX_DRAW_ITEMS 1024
#define MAX_VIEWS 10
#define MAX_RENDERABLES 1024

namespace terminus
{
    // Forward Declarations
    class  Scene;
    struct Entity;
    struct SceneView;
    struct Renderable;
    
    // Type Definitions
    
    using DrawItemArray   = std::array<DrawItem, MAX_DRAW_ITEMS>;
    using SceneViewArray  = std::array<SceneView, MAX_VIEWS>;
    using RenderableArray = std::array<Renderable, MAX_RENDERABLES>;
    
    struct RenderPrepareTaskData
    {
        int _scene_index;
    };
    
    struct Renderable
    {
        Mesh*               _mesh;
        bool                _sub_mesh_cull;
        float               _radius;
		Matrix4*			_transform;
		Vector3*			_position;
        RenderableType		_type;
        // TODO : Accomodate material overrides.
        // TODO : Union containing Renderable type (Mesh, Ocean, Terrain etc)
    };
    
    struct SceneView
    {
        DrawItemArray  _draw_items;
        int            _num_items;
        Matrix4*       _view_matrix;
        Matrix4*       _projection_matrix;
        Matrix4*       _view_projection_matrix;
        Vector4        _screen_rect;
        Texture*       _render_target;
        bool           _is_shadow;
        uint32         _cmd_buf_idx;
        RenderingPath* _rendering_path;
    };
    
    class RenderSystem
    {
    private:
        SceneViewArray	   _views;
        uint16			   _view_count;
        RenderableArray	   _renderables;
        uint16			   _renderable_count;
        DefaultThreadPool* _thread_pool;
        Renderer*		   _renderer;
        ShaderCache*	   _shader_cache;
		Scene*			   _scene;
        
    public:
        RenderSystem();
        ~RenderSystem();
        void initialize();
        void update(double delta);
        void shutdown();
        void on_entity_created(Entity entity);
        void on_entity_destroyed(Entity entity);
        
    private:
        TASK_METHOD_DECLARATION(RenderPrepareTask);
    };
}

#endif
