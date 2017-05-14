#include <gameplay/render_system.h>
#include <gameplay/scene.h>
#include <gameplay/component_types.h>
#include <resource/shader_cache.h>
#include <core/context.h>
#include <core/frame_packet.h>
#include <utility/profiler.h>
#include <resource/mesh.h>

TERMINUS_BEGIN_NAMESPACE

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{
    
}

void RenderSystem::initialize(Scene* scene)
{
    m_scene = scene;
    
	CameraComponent* camera_array = m_scene->_camera_pool.get_array();
	uint32_t num_cameras = m_scene->_camera_pool.get_num_objects();
    
    m_skydome_mesh = context::get_mesh_cache().load("sky_dome.tsm");
    
    if(scene->_sky_pool.get_num_objects() > 0)
        m_sky_cmp = scene->_sky_pool.get_array();
    else
        m_sky_cmp = nullptr;
    
    Renderer* renderer = &context::get_renderer();
    
    // TODO : For each Shadow Camera, add SceneView

    for (int i = 0; i < m_scene->_entities._num_objects; i++)
    {
		Entity& entity = m_scene->_entities._objects[i];
        on_entity_created(entity);
    }
    
    m_skydome_item.base_index = m_skydome_mesh->sub_meshes[0].base_index;
    m_skydome_item.base_vertex = m_skydome_mesh->sub_meshes[0].base_vertex;
    m_skydome_item.index_count = m_skydome_mesh->sub_meshes[0].index_count;
    m_skydome_item.material = m_skydome_mesh->sub_meshes[0].material;
    m_skydome_item.vertex_array = m_skydome_mesh->vertex_array;
    m_skydome_item.type = RenderableType::Skybox;
    
    ShaderCache* shader_cache = &context::get_shader_cache();
	SceneView* views = m_scene_views.array();
    
    for(int i = 0; i < m_scene_views.size(); i++)
    {
        RenderingPath* path = views[i].rendering_path;
        
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

void RenderSystem::simulate(FramePacket* pkt, double delta)
{
	StaticRenderable* renderables = m_static_renderables.array();

	for (uint32_t i = 0; i < m_static_renderables.size(); i++)
	{
		StaticRenderable& renderable = renderables[i];
		TransformComponent& transform_component = m_scene->get_transform_component(renderable.entity);
		renderable.transform = transform_component._global_transform;
		renderable.position = transform_component._position;
	}

	SceneView* views = m_scene_views.array();

	for (uint32_t i = 0; i < m_scene_views.size(); i++)
	{
		SceneView& view = views[i];
		CameraComponent& camera_component = m_scene->get_camera_component(view.camera_entity);
		view.screen_rect = camera_component.screen_rect;
		view.projection_matrix = camera_component.projection_matrix;
		view.view_matrix = camera_component.view_matrix;
		view.view_projection_matrix = camera_component.view_projection_matrix;
	}

	if (pkt)
	{
		SceneRenderState& render_state = pkt->scene_render_states[pkt->scene_count];

		// Copy Renderable Array
		memcpy(&render_state.static_renderables[0],
			   m_static_renderables.array(), 
			   sizeof(StaticRenderable) * m_static_renderables.size());

		render_state.num_renderables = m_static_renderables.size();
		render_state.view_count = m_scene_views.size();

		SceneView* views = m_scene_views.array();

		for (uint32_t i = 0; i < m_scene_views.size(); i++)
			views[i].scene_index = pkt->scene_count;

		// Copy Scene View Array
		memcpy(&pkt->views[pkt->total_views],
			   m_scene_views.array(), 
			   sizeof(SceneView) * m_scene_views.size());

		pkt->total_views += m_scene_views.size();

		pkt->scene_count++;
	}
}

void RenderSystem::shutdown()
{
	StaticRenderable* renderable = m_static_renderables.array();

    for (int i = 0; i < m_scene_views.size(); i++)
    {
        // unload mesh
        MeshCache& cache = context::get_mesh_cache();
        cache.unload(renderable[i].mesh);
    }
}

void RenderSystem::on_entity_created(Entity entity)
{
    if(m_scene->has_transform_component(entity) && m_scene->has_mesh_component(entity))
    {
        MeshComponent& mesh_component = m_scene->get_mesh_component(entity);
        TransformComponent& transform_component = m_scene->get_transform_component(entity);
        
		StaticRenderable* renderable = m_static_renderables.add(entity);

		renderable->entity = entity;
        renderable->mesh = mesh_component.mesh;
        renderable->sub_mesh_cull = mesh_component.cull_submeshes;
        // TODO : assign Radius. Add to MeshImporter.
        renderable->transform = transform_component._global_transform;
        renderable->position = transform_component._position;
        
        if(mesh_component.mesh->is_skeletal)
            renderable->type = RenderableType::SkeletalMesh;
        else if(mesh_component.mesh->is_skeletal)
            renderable->type = RenderableType::StaticMesh;
        
        ShaderCache* shader_cache = &context::get_shader_cache();
        
		SceneView* views = m_scene_views.array();

        for(int i = 0; i < m_scene_views.size(); i++)
        {
            RenderingPath* path = views[i].rendering_path;
            
            for (int j = 0; j < path->_num_render_passes; j++)
            {
                RenderPass* render_pass = path->_render_passes[j];
                
                if(render_pass->geometry_type == GeometryType::SCENE)
                {
                    for(RenderSubPass& sub_pass : render_pass->sub_passes)
                    {
                        for (int x = 0; x < renderable->mesh->mesh_count; x++)
                        {
                            Material* mat = renderable->mesh->sub_meshes[x].material;
                            shader_cache->load(renderable->type, render_pass->pass_id, &sub_pass, mat);
                        }
                    }
                }
            }
        }
    }
	if (m_scene->has_camera_component(entity))
	{
		CameraComponent& camera_component = m_scene->get_camera_component(entity);
		
		SceneView* view = m_scene_views.add(entity);
	
		view->camera_entity = entity;
		view->screen_rect = camera_component.screen_rect;
		view->is_shadow = false;
		view->rendering_path = camera_component.rendering_path;
		view->projection_matrix = camera_component.projection_matrix;
		view->view_matrix = camera_component.view_matrix;
		view->view_projection_matrix = camera_component.view_projection_matrix;
	}
	if (m_scene->has_point_light_component(entity))
	{

	}
}

void RenderSystem::on_entity_destroyed(Entity entity)
{
	m_scene_views.remove(entity);
	m_static_renderables.remove(entity);
}

TERMINUS_END_NAMESPACE

