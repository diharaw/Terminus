#pragma once

#include <physics/physics_types.h>

namespace terminus
{
    struct PhysicsEngineState
    {
        btDynamicsWorld* _dynamics_world;
        btDispatcher* _dispatcher;
        btBroadphaseInterface* _broadphase;
        btConstraintSolver* _constraint_solver;
        btCollisionConfiguration* _collision_config;
    };
    
    namespace physics
    {
        extern void initialize();
        extern void update(double dt);
        extern void shutdown();
        extern void set_gravity(float gravity);
    
        
    }
}
