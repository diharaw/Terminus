#pragma once

#ifndef  SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "Component.h"
#include "../Types.h"

namespace Terminus { namespace ECS {

	using Entity = uint32;

	struct ScriptComponent : IComponent
	{
        static const ComponentID _id;
		virtual void Initialize() = 0;
		virtual void Update(double delta, Entity entity) = 0;
		virtual void Shutdown() = 0;
	};

} }

#endif
