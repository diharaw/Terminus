#include <gameplay/physics_system.h>
#include <gameplay/scene.h>
#include <physics/physics_engine.h>
#include <core/context.h>

namespace terminus
{
    PhysicsSystem::PhysicsSystem()
    {
        
    }
    
    PhysicsSystem::~PhysicsSystem()
    {
        
    }
    
    void PhysicsSystem::initialize(Scene* scene)
    {
        _scene = scene;
        Entity* array = _scene->get_entity_array();
        
        for(int i = 0; i < _scene->get_num_entities(); i++)
        {
            ID transform = _scene->get_transform_id(array[i]);
            ID rigid_body = _scene->get_rigid_body_id(array[i]);
            
            if(transform != INVALID_ID && rigid_body != INVALID_ID)
            {
                ID pair_id = _pairs.add();
                _entities[INDEX_FROM_ID(array[i]._id)] = pair_id;
                RigidBodyTransform& pair = _pairs.lookup(pair_id);
                pair.rigid_body = rigid_body;
                pair.transform = transform;
            }
        }
    }
    
    void PhysicsSystem::update(double delta)
    {
        PhysicsEngineState& state = context::get_physics_state();
        
        for(int i = 0; i < _pairs.size(); i++)
        {
            TransformComponent& transform = _scene->_transform_pool._pool.lookup(_pairs._objects[i].transform);
            RigidBodyComponent& rigid_body = _scene->_rigid_body_pool._pool.lookup(_pairs._objects[i].rigid_body);
            
            transform._global_transform = physics::get_world_transform(rigid_body._rigid_body, state);
            transform::set_position(transform, physics::get_position(rigid_body._rigid_body, state));
            transform::set_rotation_quat(transform, physics::get_rotation(rigid_body._rigid_body, state));
        }
    }
    
    void PhysicsSystem::shutdown()
    {
        
    }
    
    void PhysicsSystem::on_entity_created(Entity entity)
    {
        
    }
    
    void PhysicsSystem::on_entity_destroyed(Entity entity)
    {
        
    }
}
