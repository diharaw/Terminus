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
		Vector3 offset;
		bool inherit_rotation_x;
		bool inherit_rotation_y;
		bool inherit_rotation_z;
        bool is_offscreen;
        bool is_active;
        Vector4 screen_rect;
	};

} }

#endif
