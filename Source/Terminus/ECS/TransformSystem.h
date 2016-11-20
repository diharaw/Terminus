#pragma once

#ifndef  TRANSFORMSYSTEm_H
#define TRANSFORMSYSTEm_H

#include "System.h"
#include "TransformComponent.h"

namespace Terminus { namespace ECS {

	class TransformSystem : public ISystem
	{
	public:
		TransformSystem(World* world);
		~TransformSystem();
		virtual void Initialize();
		virtual void Update(double delta);
		virtual void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};

} }

#endif


