#include <physics/physics_engine.h>
#include <core/context.h>

namespace terminus
{
    namespace physics
    {
        void initialize()
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            state._collision_config = new btDefaultCollisionConfiguration();
            state._dispatcher = new btCollisionDispatcher(state._collision_config);
            state._broadphase = new btDbvtBroadphase();
            state._constraint_solver = new btSequentialImpulseConstraintSolver();
            state._dynamics_world = new btDiscreteDynamicsWorld(state._dispatcher,
                                                                state._broadphase,
                                                                state._constraint_solver,
                                                                state._collision_config);
        }
        
        void update(double dt)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            state._dynamics_world->stepSimulation(dt);
        }
        
        void shutdown()
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            for(int i = 0; i < state._dynamics_world->getNumCollisionObjects(); i++)
            {
                btCollisionObject* obj = state._dynamics_world->getCollisionObjectArray()[i];
                state._dynamics_world->removeCollisionObject(obj);
            }
            
            T_SAFE_DELETE(state._dynamics_world);
            T_SAFE_DELETE(state._constraint_solver);
            T_SAFE_DELETE(state._broadphase);
            T_SAFE_DELETE(state._dispatcher);
            T_SAFE_DELETE(state._collision_config);
        }
        
    }
}
