#include <gameplay/camera_system.h>
#include <gameplay/scene.h>
#include <math/math_utility.h>

namespace terminus
{
	CameraSystem::CameraSystem()
	{
        _num_entities = 0;
        
        for(int i = 0; i < 16; i++)
            _entities[i]._id = INVALID_ID;
	}

	CameraSystem::~CameraSystem()
	{

	}

	void CameraSystem::initialize(Scene* scene)
	{
        _scene = scene;
        Entity* entity_array = _scene->get_entity_array();
        
        for (int i = 0; i < _scene->get_num_entities(); i++)
        {
            Entity& entity = entity_array[i];
            on_entity_created(entity);
        }
	}

	void CameraSystem::update(double delta)
	{
        for(int i = 0; i < 16; i++)
        {
            if(_entities[i]._id != INVALID_ID)
            {
                TransformComponent& trn_cmp = _scene->get_transform_component(_entities[i]);
                CameraComponent& cmr_cmp = _scene->get_camera_component(_entities[i]);
                
                //cmr_cmp.camera.SetPosition(trn_cmp._position);
                //cmr_cmp.camera.Update();
                
				cmr_cmp.transform._forward = glm::conjugate(cmr_cmp.transform._rotation) * Vector3(0.0f, 0.0f, -1.0f);
				cmr_cmp.transform._forward = glm::normalize(cmr_cmp.transform._forward);
                // inherit position;
                Vector3 final_pos = cmr_cmp.transform._position + trn_cmp._position;
                
                Vector3 final_rot = cmr_cmp.transform._euler_angles;

                // TODO: move into Transform System
                Matrix4 entity_translation = glm::translate(-trn_cmp._position);
                Matrix4 camera_translation = glm::translate(-cmr_cmp.transform._position);
                Matrix4 rotation = glm::toMat4(cmr_cmp.transform._rotation);
                
                cmr_cmp.transform._global_transform = glm::translate(trn_cmp._position) * glm::translate(cmr_cmp.transform._position);
                //cmr_cmp.view_matrix = camera_translation * rotation * entity_translation;
				cmr_cmp.view_matrix = rotation * camera_translation;
                cmr_cmp.view_projection_matrix = cmr_cmp.projection_matrix * cmr_cmp.view_matrix;
            }
        }
	}

	void CameraSystem::shutdown()
	{

	}

	void CameraSystem::on_entity_created(Entity entity)
	{
        //if(_scene->has_camera_component(entity) && _scene->has_transform_component(entity))
          //  _entities[_num_entities++] = entity;
        
        if(_scene->has_camera_component(entity) && _scene->has_transform_component(entity))
        {
            _entities[_num_entities++] = entity;
            
            TransformComponent& trn_cmp = _scene->get_transform_component(entity);
            CameraComponent& cmr_cmp = _scene->get_camera_component(entity);
           
            // inherit position;
            Vector3 final_pos = cmr_cmp.transform._position + trn_cmp._position;
            
            Vector3 final_rot = cmr_cmp.transform._euler_angles;
  
            cmr_cmp.transform._rotation = glm::quat(Vector3(glm::radians(final_rot.x),
                                                            glm::radians(final_rot.y),
                                                            glm::radians(final_rot.z)));

			cmr_cmp.transform._forward = glm::conjugate(cmr_cmp.transform._rotation) * Vector3(0.0f, 0.0f, -1.0f);
			cmr_cmp.transform._forward = glm::normalize(cmr_cmp.transform._forward);
            
			Matrix4 entity_translation = glm::translate(-trn_cmp._position);
			Matrix4 camera_translation = glm::translate(-cmr_cmp.transform._position);
			Matrix4 rotation = glm::toMat4(cmr_cmp.transform._rotation);

			cmr_cmp.transform._global_transform = glm::translate(trn_cmp._position) * glm::translate(cmr_cmp.transform._position);
			//cmr_cmp.view_matrix = camera_translation * rotation * entity_translation;
			cmr_cmp.view_matrix = rotation * camera_translation;
            
            if(cmr_cmp.projection_type == ProjectionType::PERSPECTIVE)
            {
                cmr_cmp.projection_matrix = Math::Perspective(glm::radians(cmr_cmp.persp_info.fov),
                                                              cmr_cmp.persp_info.aspect_ratio,
                                                              cmr_cmp.near_plane,
                                                              cmr_cmp.far_plane);
            }
            else
            {
                cmr_cmp.projection_matrix = Math::Ortho(cmr_cmp.ortho_info.left,
                                                        cmr_cmp.ortho_info.right,
                                                        cmr_cmp.ortho_info.down,
                                                        cmr_cmp.ortho_info.up,
                                                        cmr_cmp.near_plane,
                                                        cmr_cmp.far_plane);
            }
            
            cmr_cmp.view_projection_matrix = cmr_cmp.projection_matrix * cmr_cmp.view_matrix;
        }
	}

	void CameraSystem::on_entity_destroyed(Entity entity)
	{
        if(_scene->has_camera_component(entity) && _scene->has_transform_component(entity))
        {
            for (int i = 0; i < 16; i++)
            {
                if(_entities[i]._id == entity._id)
                    _entities[i]._id = INVALID_ID;
            }
        }
	}
}
