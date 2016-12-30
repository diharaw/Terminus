#include "RenderSystem.h"
#include "Scene.h"
#include "CameraComponent.h"

namespace Terminus { namespace ECS {
    
    // Sort Method
    
    bool DrawItemSort(Graphics::DrawItem i, Graphics::DrawItem j)
    {
        return i.sort_key.key > j.sort_key.key;
    }

    RenderSystem::RenderSystem()
    {
        m_view_count = 0;
        
        m_thread_pool = Global::GetDefaultThreadPool();
    }
    
    RenderSystem::~RenderSystem()
    {
        
    }
    
    void RenderSystem::Initialize()
    {
        SlotMap<CameraComponent, MAX_COMPONENTS>& component_list = m_scene->GetComponentArray<CameraComponent>();
        
        // For each Scene Camera
        for (int i = 0; i < component_list._num_objects; i++)
        {
            SceneView& view = m_views[m_view_count++];
            view._screen_rect = component_list._objects[i].screen_rect;
            view._is_shadow = false;
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (Entity entity : m_scene->m_entities)
        {
            MeshComponent* mesh_component = (MeshComponent*)m_scene->GetComponent(entity, TransformComponent::_id);
            TransformComponent* transform_component = (TransformComponent*)m_scene->GetComponent(entity, TransformComponent::_id);
            
            Renderable& renderable = m_renderables[m_renderable_count++];
            
            renderable._mesh = mesh_component->mesh;
            renderable._sub_mesh_cull = mesh_component->cull_submeshes;
            // TODO : assign Radius. Add to MeshImporter.
            renderable._transform = transform_component;
            
            
            // Commented for now.
            
//            if(mesh_component && transform_component)
//            {
//                for (auto view : m_views)
//                {
//                    if((view._is_shadow && mesh_component->casts_shadow) || !view._is_shadow)
//                    {
//                        Graphics::DrawItem& draw_item = view._draw_items[view._num_items++];
//                        draw_item.vertex_array = mesh_component->mesh->VertexArray;
//                        
//                        if(mesh_component->mesh->IsSkeletal)
//                            draw_item.type = Graphics::RenderableType::SkeletalMesh;
//                        else
//                            draw_item.type = Graphics::RenderableType::StaticMesh;
//                        
//                        draw_item.transform = &transform_component->global_transform;
//                    }
//                }
//            }
        }
    }
    
    void RenderSystem::Update(double delta)
    {
        int worker_count = m_thread_pool->WorkerThreadCount();
        LinearAllocator* per_frame_alloc = Global::GetPerFrameAllocator();
        
        // Assign views to threads and frustum cull, sort and fill command buffers in parallel.
        
        int items_per_thread = std::floor((float)m_view_count / (float)worker_count);
        
        if(items_per_thread == 0)
            items_per_thread = m_view_count;
        
        int submitted_items = 0;
        int scene_index = 0;
        bool is_done = false;
        
        while(!is_done)
        {
            int remaining_items = m_view_count - submitted_items;
            
            if(remaining_items <= items_per_thread)
            {
                is_done = true;
                items_per_thread = remaining_items;
            }
            
            TaskData* task = m_thread_pool->CreateTask();
            RenderPrepareTaskData* data = per_frame_alloc->NewPerFrame<RenderPrepareTaskData>();
    
            data->_scene_index = scene_index++;
            
            task->data = data;
            task->function.Bind<RenderSystem, &RenderSystem::RenderPrepareTask>(this);
            
            submitted_items += items_per_thread;
        }
        
        m_thread_pool->SubmitAndWait();
    }
    
    void RenderSystem::Shutdown()
    {
        
    }
    
    void RenderSystem::OnEntityCreated(Entity entity)
    {
        
    }
    
    void RenderSystem::OnEntityDestroyed(Entity entity)
    {
        
    }
    
    TASK_METHOD_DEFINITION(RenderSystem, RenderPrepareTask)
    {
        RenderPrepareTaskData* _data = static_cast<RenderPrepareTaskData*>(data);
        SceneView& scene_view = m_views[_data->_scene_index];
        
        // Frustum cull Renderable array and fill DrawItem array
        
        for (int i = 0; i < m_renderable_count; i++)
        {
            Renderable& renderable = m_renderables[i];
        }
        
        // Sort DrawItem array
        
        std::partial_sort(scene_view._draw_items.begin(),
                          scene_view._draw_items.begin() + scene_view._num_items,
                          scene_view._draw_items.end(),
                          DrawItemSort);
        
        // Fill CommandBuffer while skipping redundant state changes
    }
    
} }

