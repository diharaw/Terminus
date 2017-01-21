#pragma once

#include <Graphics/camera.h>
#include <Graphics/rendering_path.h>
#include <Resource/mesh.h>
#include <script/cpp_script.h>
#include <ECS/entity.h>
#include <types.h>

#include <string>

namespace terminus
{
	// forward declaration
	class Scene;

	struct ColliderComponent
	{
		
	};

	struct CameraComponent
	{
		Camera camera;
		Vector3 offset;
		bool inherit_rotation_x;
		bool inherit_rotation_y;
		bool inherit_rotation_z;
		bool is_offscreen;
		bool is_active;
		Vector4 screen_rect;
        Texture* render_target;
        RenderingPath* rendering_path;
	};

	struct MeshComponent
	{
		Mesh* mesh;
		bool cull_submeshes;
		bool casts_shadow;
		bool is_visible;
	};

	struct TransformComponent
	{
		Matrix4 _global_transform;
		Vector3 _position;
		Vector3 _scale;
		Vector3 _rotation;
		bool    _is_dirty;
		String	_parent_entity_name;
	};

	struct LuaScriptComponent
	{
		Entity		_entity;
		Scene*		_scene;
		LuaFunction _initialize;
		LuaFunction _update;
		LuaFunction _shutdown;
		LuaObject	_object;

        void initialize();

        void update(double dt);

        void shutdown();
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

	inline void set_rotation(TransformComponent& cmp, Vector3& rot)
	{
		cmp._rotation = rot;
		cmp._is_dirty = true;
	}

}
