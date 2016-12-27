#include "RenderSystem.h"
#include "Scene.h"
#include "CameraComponent.h"

namespace Terminus { namespace ECS {

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
            view.screen_rect = component_list._objects[i].screen_rect;
        }
        
        // TODO : For each Shadow Camera, add SceneView

        for (Entity entity : m_scene->m_entities)
        {
            MeshComponent* mesh_component = (MeshComponent*)m_scene->GetComponent(entity, TransformComponent::_id);
            TransformComponent* transform_component = (TransformComponent*)m_scene->GetComponent(entity, TransformComponent::_id);
            
            if(mesh_component && transform_component)
            {
                for (int i = 0; i < m_view_count; i++)
                {
                    if((m_views[i].is_shadow && mesh_component->casts_shadow) || !m_views[i].is_shadow)
                    {
                        DrawItem& draw_item = m_views[i].visible_draw_items[m_views[i].visible_item_count++];
                        draw_item.mesh = mesh_component->mesh;
                        
                        if(mesh_component->mesh->IsSkeletal)
                            draw_item.type = DrawItemType::SkeletalMesh;
                        else
                            draw_item.type = DrawItemType::StaticMesh;
                        
                        draw_item.transform = &transform_component->global_transform;
                    }
                }
            }
        }
    }
    
    void RenderSystem::Update(double delta)
    {
        int worker_count = m_thread_pool->WorkerThreadCount();
        LinearAllocator* per_frame_alloc = Global::GetPerFrameAllocator();
        
        // Frustum Cull each view parallely
        for (int i = 0; i < m_view_count; i++)
        {
            int work_group_size = m_views[i].visible_item_count/worker_count;
            
            if(m_views[i].visible_item_count < worker_count)
            {
                FrustumCullTaskData* data = per_frame_alloc->NewPerFrame<FrustumCullTaskData>();
                work_group_size = m_views[i].visible_item_count;
                
                data->start_index = 0;
                data->item_count = work_group_size;
                
                FrustumCullTask(data);
            }
            else
            {
                for(int j = 0; j < worker_count; j++)
                {
                    TaskData* task = m_thread_pool->CreateTask();
                    FrustumCullTaskData* data = per_frame_alloc->NewPerFrame<FrustumCullTaskData>();
                    
                    data->start_index = j * work_group_size;
                    data->item_count = work_group_size;
                    
                    task->data = data;
                    task->function.Bind<RenderSystem, &RenderSystem::FrustumCullTask>(this);
                }
                
                m_thread_pool->SubmitAndWait();
            }
        }
        
        // TODO: Sort Views Parallely
        
        // Generate Command List Parallely
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
    
    TASK_METHOD_DEFINITION(RenderSystem, FrustumCullTask)
    {
        
    }
    
} }

