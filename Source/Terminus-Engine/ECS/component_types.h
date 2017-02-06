#pragma once

#include <Graphics/camera.h>
#include <Graphics/rendering_path.h>
#include <Resource/mesh.h>
#include <script/cpp_script.h>
#include <ECS/entity.h>
#include <types.h>
#include <script/lua_script.h>

#include <string>

namespace terminus
{
	// forward declaration
	class Scene;

	struct ColliderComponent
	{
		
	};

//	struct CameraComponent
//	{
//		Camera camera;
//		Vector3 offset;
//		bool inherit_rotation_x;
//		bool inherit_rotation_y;
//		bool inherit_rotation_z;
//		bool is_offscreen;
//		bool is_active;
//		Vector4 screen_rect;
//        Texture* render_target;
//        RenderingPath* rendering_path;
//	};

	struct MeshComponent
	{
		Mesh* mesh;
		bool cull_submeshes;
		bool casts_shadow;
		bool is_visible;
	};

	struct TransformComponent
	{
		Matrix4    _global_transform;
		Vector3    _position;
		Vector3    _scale;
        Quaternion _rotation;
        Vector3    _forward;
        Vector3    _euler_angles;
		bool       _is_dirty;
		String     _parent_entity_name;
	};

    enum class ProjectionType
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC
    };
    
    struct PerspectiveInfo
    {
        float fov;
        float aspect_ratio;
    };
    
    struct OrthographicInfo
    {
        float left;
        float right;
        float up;
        float down;
    };
    
    struct CameraComponent
    {
        TransformComponent transform;
        Matrix4            projection_matrix;
        Matrix4            view_matrix;
        Matrix4            view_projection_matrix;
        float              near_plane;
        float              far_plane;
        bool               is_offscreen;
        bool               is_active;
        bool               inherit_pitch;
        bool               inherit_yaw;
        bool               inherit_roll;
        Vector4            screen_rect;
        Texture*           render_target;
        RenderingPath*     rendering_path;
        
        ProjectionType     projection_type;
        
        union
        {
            PerspectiveInfo  persp_info;
            OrthographicInfo ortho_info;
        };
    };
    
	struct LuaScriptComponent
	{
		Entity		_entity;
		Scene*		_scene;
        LuaScript*  _script;
	};
    
    struct CppScriptComponent
    {
        String      _class_name;
        String      _file_name;
        Entity		_entity;
        Scene*		_scene;
        CppScript*  _script;
    };

	// Transform helpers

	namespace transform
    {
        inline void set_position(TransformComponent& cmp, Vector3& pos)
        {
            cmp._position = pos;
            cmp._is_dirty = true;
        }
        
        inline void set_scale(TransformComponent& cmp, Vector3& scale)
        {
            cmp._scale = scale;
            cmp._is_dirty = true;
        }
        
        inline void set_rotation_euler(TransformComponent& cmp, Vector3& rot)
        {
            cmp._euler_angles = rot;
            cmp._rotation = glm::quat(Vector3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z)));
            cmp._is_dirty = true;
        }
        
        inline void set_rotation_quat(TransformComponent& cmp, Quaternion& rot)
        {
            cmp._rotation = rot;
            Vector3 euler = glm::eulerAngles(cmp._rotation);
            cmp._euler_angles = Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
            cmp._is_dirty = true;
        }
        
        inline void look_at(TransformComponent& cmp, Vector3& point)
        {
            Matrix4 rotation = glm::lookAt(cmp._position, point, Vector3(0.0, 1.0, 0.0));
            //cmp._rotation = glm::conjugate(glm::toQuat(rotation));
            cmp._rotation = glm::quat(rotation);
            Vector3 euler = glm::eulerAngles(cmp._rotation);
            cmp._euler_angles = Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
            cmp._is_dirty = true;
        }
    }
    
    namespace camera
    {
        // relative position
        inline void set_position(CameraComponent& cmp, Vector3& pos)
        {
            transform::set_position(cmp.transform, pos);
        }
        
        inline void set_yaw(CameraComponent& cmp, float yaw)
        {
            cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
                                                  yaw,
                                                  cmp.transform._euler_angles.z);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
        
        inline void set_pitch(CameraComponent& cmp, float pitch)
        {
            cmp.transform._euler_angles = Vector3(pitch,
                                                  cmp.transform._euler_angles.y,
                                                  cmp.transform._euler_angles.z);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
        
        inline void set_roll(CameraComponent& cmp, float roll)
        {
            cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
                                                  cmp.transform._euler_angles.y,
                                                  roll);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
        
        inline void offset_position(CameraComponent& cmp, Vector3& pos)
        {
            cmp.transform._euler_angles = cmp.transform._position + pos;
            //transform::set_position(cmp.transform, new_pos);
        }
        
        inline void offset_yaw(CameraComponent& cmp, float yaw)
        {
            cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
                                                  cmp.transform._euler_angles.y + yaw,
                                                  cmp.transform._euler_angles.z);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
        
        inline void offset_pitch(CameraComponent& cmp, float pitch)
        {
            cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x + pitch,
                                    cmp.transform._euler_angles.y,
                                    cmp.transform._euler_angles.z);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
        
        inline void offset_roll(CameraComponent& cmp, float roll)
        {
            cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
                                    cmp.transform._euler_angles.y,
                                    cmp.transform._euler_angles.z + roll);
            //transform::set_rotation_euler(cmp.transform, euler);
        }
    }
}
