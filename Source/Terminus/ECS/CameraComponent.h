#pragma once

#ifndef  CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "../Graphics/Camera.h"
#include "Component.h"

namespace Terminus {  namespace ECS {

	struct CameraComponent : IComponent
	{
        static const ComponentID _id;
		Graphics::Camera* camera;
		Vector3				     offset;
		bool						     inherit_rotation_x;
		bool							 inherit_rotation_y;
		bool							 inherit_rotation_z;
	};

	const ComponentID CameraComponent::_id = 1;

} } 

#endif
