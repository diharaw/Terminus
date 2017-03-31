#pragma once

#include <graphics/camera.h>
#include <graphics/rendering_path.h>
#include <resource/mesh.h>
#include <script/cpp_script.h>
#include <gameplay/entity.h>
#include <core/types.h>
#include <script/lua_script.h>
#include <physics/physics_types.h>

#include <string>

namespace terminus
{
	// forward declaration
	class Scene;

    struct SkyComponent
    {
        StringBuffer32 cubemap;
        TextureCube*   texture;
    };
    
    struct PointLightComponent
    {
        Vector4 color;
        float intensity;
        bool casts_shadow;
        Vector3 position;
        float constant_attenuation;
        float linear_attenuation;
        float quadratic_attenuation;
    };
    
    struct SpotLightComponent
    {
        Vector4 color;
        float intensity;
        bool casts_shadow;
        Vector3 position;
        Vector3 direction;
        float constant_attenuation;
        float linear_attenuation;
        float quadratic_attenuation;
        float inner_cutoff;
        float outer_cutoff;
    };
    
    struct DirectionalLightComponent
    {
        Vector4 color;
        float intensity;
        bool casts_shadow;
        Vector3 direction;
    };
    
    struct RigidBodyComponent
    {
        ID     _rigid_body;
        bool   _is_kinematic;
        Entity _entity;
    };
    
    struct ColliderComponent
    {
        CollisionShapeType _type;
        
        union
        {
            SphereShape       _sphere;
            BoxShape          _box;
            CylinderShape     _cylinder;
            CapsuleShape      _capsule;
            TriangleMeshShape _mesh;
            ConvexHullShape   _hull;
            StaticPlaneShape  _plane;
        };
        
        ColliderComponent()
        {
            
        }
    };

	struct SphereColliderComponent
	{
        SphereShape _collision_shape;
	};
    
    struct BoxColliderComponent
    {
        BoxShape _collision_shape;
    };
    
    struct CylinderColliderComponent
    {
        CylinderShape _collision_shape;
    };
    
    struct CapsuleColliderComponent
    {
        CapsuleShape _collision_shape;
    };
    
    struct HeightFieldColliderComponent
    {
        CapsuleShape _collision_shape;
    };
    
    struct TriangleMeshColliderComponent
    {
        TriangleMeshShape _collision_shape;
    };
    
    struct ConvexHullColliderComponent
    {
        ConvexHullShape _collision_shape;
    };
    
    struct PlaneColliderComponent
    {
        StaticPlaneShape _collision_shape;
    };

    struct MeshComponent
	{
#if defined(TERMINUS_WITH_EDITOR)
		StringBuffer32 mesh_name;
#endif
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
		Vector3	   _right;
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
		float aspect_ratio_x;
		float aspect_ratio_y;
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

    struct AudioListenerComponent
    {
        
    };
    
    struct AudioSourceComponent
    {
        
    };
    
	// Transform helpers

	namespace transform
	{
		inline void set_position(TransformComponent& cmp, Vector3 pos)
		{
			cmp._position = pos;
			cmp._is_dirty = true;
		}

		inline void offset_position(TransformComponent& cmp, Vector3& pos)
		{
			cmp._position += pos;
			cmp._is_dirty = true;
		}

		inline void set_scale(TransformComponent& cmp, Vector3 scale)
		{
			cmp._scale = scale;
			cmp._is_dirty = true;
		}

		inline void set_rotation_euler(TransformComponent& cmp, Vector3 rot)
		{
			cmp._euler_angles = rot;
			cmp._rotation = glm::quat(Vector3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z)));
			cmp._is_dirty = true;
		}

		inline void set_rotation_quat(TransformComponent& cmp, Quaternion rot)
		{
			cmp._rotation = rot;
			Vector3 euler = glm::eulerAngles(cmp._rotation);
			cmp._euler_angles = Vector3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
			cmp._is_dirty = true;
		}
        
        inline void set_transform(TransformComponent& cmp, Matrix4 transform)
        {
            cmp._global_transform = transform;
        }

		inline void look_at(TransformComponent& cmp, Vector3 point)
		{
			Matrix4 rotation = glm::lookAt(cmp._position, point, Vector3(0.0, 1.0, 0.0));

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

		inline void set_euler(CameraComponent& cmp, Vector3 euler)
		{
			cmp.transform._euler_angles = euler;

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void set_yaw(CameraComponent& cmp, float yaw)
		{
			cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
				yaw,
				cmp.transform._euler_angles.z);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void set_pitch(CameraComponent& cmp, float pitch)
		{
			cmp.transform._euler_angles = Vector3(pitch,
				cmp.transform._euler_angles.y,
				cmp.transform._euler_angles.z);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void set_roll(CameraComponent& cmp, float roll)
		{
			cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
				cmp.transform._euler_angles.y,
				roll);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void offset_position(CameraComponent& cmp, Vector3& pos)
		{
			cmp.transform._euler_angles = cmp.transform._position + pos;
		}

		inline void offset_euler(CameraComponent& cmp, Vector3 euler)
		{
			cmp.transform._euler_angles += euler;

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void offset_yaw(CameraComponent& cmp, float yaw)
		{
			cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
				cmp.transform._euler_angles.y + yaw,
				cmp.transform._euler_angles.z);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void offset_pitch(CameraComponent& cmp, float pitch)
		{
			cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x + pitch,
				cmp.transform._euler_angles.y,
				cmp.transform._euler_angles.z);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}

		inline void offset_roll(CameraComponent& cmp, float roll)
		{
			cmp.transform._euler_angles = Vector3(cmp.transform._euler_angles.x,
				cmp.transform._euler_angles.y,
				cmp.transform._euler_angles.z + roll);

			Quaternion qPitch = glm::angleAxis(glm::radians(cmp.transform._euler_angles.x), Vector3(1, 0, 0));
			Quaternion qYaw = glm::angleAxis(glm::radians(cmp.transform._euler_angles.y), Vector3(0, 1, 0));
			Quaternion qRoll = glm::angleAxis(glm::radians(cmp.transform._euler_angles.z), Vector3(0, 0, 1));

			cmp.transform._rotation = qPitch * qYaw * qRoll;
			cmp.transform._rotation = glm::normalize(cmp.transform._rotation);
		}
	}

#if defined(TERMINUS_WITH_EDITOR)

	inline JsonValue component_serialize(TransformComponent& cmp, JsonDocument& doc)
	{
		JsonValue json_cmp(rapidjson::kObjectType);

		// type
		JsonValue cmp_type("TRANSFORM", doc.GetAllocator());
		json_cmp.AddMember("component_type", cmp_type, doc.GetAllocator());

		// position
		JsonValue position(rapidjson::kObjectType);

		position.AddMember("x", cmp._position.x, doc.GetAllocator());
		position.AddMember("y", cmp._position.y, doc.GetAllocator());
		position.AddMember("z", cmp._position.z, doc.GetAllocator());

		json_cmp.AddMember("position", position, doc.GetAllocator());

		// scale
		JsonValue scale(rapidjson::kObjectType);

		scale.AddMember("x", cmp._scale.x, doc.GetAllocator());
		scale.AddMember("y", cmp._scale.y, doc.GetAllocator());
		scale.AddMember("z", cmp._scale.z, doc.GetAllocator());

		json_cmp.AddMember("scale", scale, doc.GetAllocator());

		// rotation
		JsonValue rotation(rapidjson::kObjectType);

		rotation.AddMember("x", cmp._rotation.x, doc.GetAllocator());
		rotation.AddMember("y", cmp._rotation.y, doc.GetAllocator());
		rotation.AddMember("z", cmp._rotation.z, doc.GetAllocator());

		json_cmp.AddMember("rotation", rotation, doc.GetAllocator());

		return json_cmp;
	}

	inline JsonValue component_serialize(CameraComponent& cmp, JsonDocument& doc)
	{
		JsonValue json_cmp(rapidjson::kObjectType);

		// type
		JsonValue cmp_type("CAMERA", doc.GetAllocator());
		json_cmp.AddMember("component_type", cmp_type, doc.GetAllocator());

		// transform
		JsonValue transform(rapidjson::kObjectType);

		// position
		JsonValue position(rapidjson::kObjectType);

		position.AddMember("x", cmp.transform._position.x, doc.GetAllocator());
		position.AddMember("y", cmp.transform._position.y, doc.GetAllocator());
		position.AddMember("z", cmp.transform._position.z, doc.GetAllocator());

		transform.AddMember("position", position, doc.GetAllocator());

		// rotation
		JsonValue rotation(rapidjson::kObjectType);

		rotation.AddMember("x", cmp.transform._euler_angles.x, doc.GetAllocator());
		rotation.AddMember("y", cmp.transform._euler_angles.y, doc.GetAllocator());
		rotation.AddMember("z", cmp.transform._euler_angles.z, doc.GetAllocator());

		transform.AddMember("rotation", rotation, doc.GetAllocator());

		json_cmp.AddMember("transform", transform, doc.GetAllocator());

		// projection info
		JsonValue projection_info(rapidjson::kObjectType);

		projection_info.AddMember("near_plane", cmp.near_plane, doc.GetAllocator());
		projection_info.AddMember("far_plane", cmp.far_plane, doc.GetAllocator());

		if (cmp.projection_type == ProjectionType::PERSPECTIVE)
		{
			JsonValue projection_type("PERSPECTIVE", doc.GetAllocator());
			projection_info.AddMember("projection_type", cmp_type, doc.GetAllocator());

			projection_info.AddMember("field_of_view", cmp.persp_info.fov, doc.GetAllocator());
			projection_info.AddMember("aspect_ratio_x", cmp.persp_info.aspect_ratio_x, doc.GetAllocator());
			projection_info.AddMember("aspect_ratio_y", cmp.persp_info.aspect_ratio_y, doc.GetAllocator());
		}
		else
		{
			JsonValue projection_type("ORTHOGRAPHIC", doc.GetAllocator());
			projection_info.AddMember("projection_type", projection_type, doc.GetAllocator());
		}
	
		json_cmp.AddMember("projection_info", projection_info, doc.GetAllocator());

		JsonValue rendering_path(cmp.rendering_path->_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("rendering_path", cmp_type, doc.GetAllocator());

		json_cmp.AddMember("is_active", cmp.is_active, doc.GetAllocator());
		json_cmp.AddMember("is_offscreen", cmp.is_offscreen, doc.GetAllocator());

		// viewport rect
		JsonValue viewport_rect(rapidjson::kObjectType);

		viewport_rect.AddMember("x", cmp.screen_rect.x, doc.GetAllocator());
		viewport_rect.AddMember("y", cmp.screen_rect.y, doc.GetAllocator());
		viewport_rect.AddMember("w", cmp.screen_rect.z, doc.GetAllocator());
		viewport_rect.AddMember("h", cmp.screen_rect.w, doc.GetAllocator());

		json_cmp.AddMember("viewport_rect", viewport_rect, doc.GetAllocator());

		return json_cmp;
	}

	inline JsonValue component_serialize(MeshComponent& cmp, JsonDocument& doc)
	{
		JsonValue json_cmp(rapidjson::kObjectType);

		JsonValue mesh_name(cmp.mesh_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("mesh", mesh_name, doc.GetAllocator());

		return json_cmp;
	}
	
	inline JsonValue component_serialize(LuaScriptComponent& cmp, JsonDocument& doc)
	{
		JsonValue json_cmp(rapidjson::kObjectType);

		JsonValue script_file(cmp._script->_script_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("script_file", script_file, doc.GetAllocator());

		JsonValue class_name(cmp._script->_class_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("class_name", class_name, doc.GetAllocator());

		return json_cmp;
	}

	inline JsonValue component_serialize(CppScriptComponent& cmp, JsonDocument& doc)
	{
		JsonValue json_cmp(rapidjson::kObjectType);

		JsonValue script_file(cmp._file_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("script_file", script_file, doc.GetAllocator());

		JsonValue class_name(cmp._class_name.c_str(), doc.GetAllocator());
		json_cmp.AddMember("class_name", class_name, doc.GetAllocator());

		return json_cmp;
	}

#endif
}
