#pragma once

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <types.h>
#include <ECS/component.h>

namespace terminus
{
	struct TransformComponent : IComponent
	{
        static const ComponentID _id;
		Matrix4 _global_transform;
		Vector3 _position;
		Vector3 _scale;
		Vector3 _rotation;
		bool    _is_dirty;
        String	_parent_entity_name;
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

#endif
