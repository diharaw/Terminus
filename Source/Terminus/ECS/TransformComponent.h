#pragma once

#ifndef  TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../Types.h"
#include "Component.h"

namespace Terminus { namespace ECS {

	struct TransformComponent : IComponent
	{
		Matrix4 global_transform;
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
	};

	const ComponentID TransformComponent::_id = 0;

} }

#endif