#pragma once

#ifndef  TRANSFORMSYSTEm_H
#define TRANSFORMSYSTEm_H

#include "System.h"

namespace Terminus { namespace ECS {

	class TransformSystem : public ISystem
	{
	public:
		TransformSystem();
		~TransformSystem();
		virtual void Initialize();
		virtual void Update(double delta);
		virtual void Shutdown();
		void OnEntityCreated(Entity entity);
		void OnEntityDestroyed(Entity entity);
	};

} }

#endif


