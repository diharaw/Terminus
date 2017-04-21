#include <gameplay/render_system.h>
#include <gameplay/scene.h>
#include <gameplay/component_types.h>
#include <resource/shader_cache.h>
#include <core/context.h>
#include <utility/profiler.h>

namespace terminus
{    
    RenderSystem::RenderSystem()
    {
        _view_count = 0;
        
        
        for (ID& entity_id : _entity_renderable_ref)
        {
            entity_id = INVALID_ID;
        }
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    void RenderSystem::initialize(Scene* scene)
    {
        _scene = scene;
        
		CameraComponent* camera_array = _scene->_camera_pool.get_array();
		int num_cameras = _scene->_camera_pool.get_num_objects();
        
        _skydome_mesh = context::get_mesh_cache().load("sky_dome.tsm");
        
        if(scene->_sky_pool.get_num_objects() > 0)
            _sky_cmp = scene->_sky_pool.get_array();
        else
            _sky_cmp = nullptr;
        
        Renderer* renderer = &context::get_renderer();
        
        // For each Scene Camera
        for (int i = 0; i < num_cameras; i++)
        {
            SceneView& view = _views[_view_count++];
            view._screen_rect = camera_array[i].screen_rect;
            view._is_shadow = false;
            view._cmd_buf_idx = renderer->create_command_buffer();
            view._rendering_path = camera_array[i].rendering_path;
            //view._projection_matrix = &camera_array[i].camera.m_ProjectionMatrix;
            //view._view_matrix = &camera_array[i].camera.m_ViewMatrix;
            //view._view_projection_matrix = &camera_array[i].camera.m_ViewProjection;
            
            view._projection_matrix = &camera_array[i].projection_matrix;
            view._view_matrix = &camera_array[i].view_matrix;
            view._view_projection_matrix = &camera_array[i].view_projection_matrix;
            
            view._num_items = 0;
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (int i = 0; i < _scene->_entities._num_objects; i++)
        {
			Entity& entity = _scene->_entities._objects[i];
            on_entity_created(entity);
        }
        
        _skydome_item.base_index = _skydome_mesh->SubMeshes[0].m_BaseIndex;
        _skydome_item.base_vertex = _skydome_mesh->SubMeshes[0].m_BaseVertex;
        _skydome_item.index_count = _skydome_mesh->SubMeshes[0].m_IndexCount;
        _skydome_item.material = _skydome_mesh->SubMeshes[0]._material;
        _skydome_item.vertex_array = _skydome_mesh->VertexArray;
        _skydome_item.type = RenderableType::Skybox;
        
        ShaderCache* shader_cache = &context::get_shader_cache();
        
        for(int i = 0; i < _view_count; i++)
        {
            RenderingPath* path = _views[i]._rendering_path;
            
            for (int j = 0; j < path->_num_render_passes; j++)
            {
                RenderPass* render_pass = path->_render_passes[j];
                
                if(render_pass->geometry_type == GeometryType::SKY)
                {
                    for(RenderSubPass& sub_pass : render_pass->sub_passes)
                        shader_cache->load(RenderableType::Skybox, render_pass->pass_id, &sub_pass, nullptr);
                }
            }
        }
    }
    
    void RenderSystem::update(double delta)
    {
    }
    
    void RenderSystem::shutdown()
    {
        for (int i = 0; i < _renderables._num_objects; i++)
        {
            // unload mesh
            MeshCache& cache = context::get_mesh_cache();
            cache.unload(_renderables._objects[i]._mesh);
        }
    }
    
    void RenderSystem::on_entity_created(Entity entity)
    {
        if(_scene->has_transform_component(entity) && _scene->has_mesh_component(entity))
        {
            MeshComponent& mesh_component = _scene->get_mesh_component(entity);
            TransformComponent& transform_component = _scene->get_transform_component(entity);
            
            ID renderable_id = _renderables.add();
            
            _entity_renderable_ref[INDEX_FROM_ID(entity._id)] = renderable_id;
            Renderable& renderable = _renderables.lookup(renderable_id);
            
            renderable._mesh = mesh_component.mesh;
            renderable._sub_mesh_cull = mesh_component.cull_submeshes;
            // TODO : assign Radius. Add to MeshImporter.
            renderable._transform = &transform_component._global_transform;
            renderable._position = &transform_component._position;
            
            if(mesh_component.mesh->IsSkeletal)
                renderable._type = RenderableType::SkeletalMesh;
            else if(mesh_component.mesh->IsSkeletal)
                renderable._type = RenderableType::StaticMesh;
            
            ShaderCache* shader_cache = &context::get_shader_cache();
            
            for(int i = 0; i < _view_count; i++)
            {
                RenderingPath* path = _views[i]._rendering_path;
                
                for (int j = 0; j < path->_num_render_passes; j++)
                {
                    RenderPass* render_pass = path->_render_passes[j];
                    
                    if(render_pass->geometry_type == GeometryType::SCENE)
                    {
                        for(RenderSubPass& sub_pass : render_pass->sub_passes)
                        {
                            for (int x = 0; x < renderable._mesh->MeshCount; x++)
                            {
                                Material* mat = renderable._mesh->SubMeshes[x]._material;
                                shader_cache->load(renderable._type, render_pass->pass_id, &sub_pass, mat);
                            }
                        }
                    }
                }
            }
        }
    }
    
    void RenderSystem::on_entity_destroyed(Entity entity)
    {
        if(_entity_renderable_ref[entity._id] != INVALID_ID)
        {
            ID renderable_id = _entity_renderable_ref[entity._id];
            
            // TODO: should i clean up resources here?
            
            _renderables.remove(renderable_id);
            _entity_renderable_ref[entity._id] = INVALID_ID;
        }
    }
    
    SceneView* RenderSystem::scene_views()
    {
        return &_views[0];
    }
    
    uint16_t RenderSystem::view_count()
    {
        return _view_count;
    }
    
    Renderable* RenderSystem::renderables()
    {
        return &_renderables._objects[0];
    }
    
    uint32_t RenderSystem::renderable_count()
    {
        return _renderables.size();
    }
} // namespace terminus

