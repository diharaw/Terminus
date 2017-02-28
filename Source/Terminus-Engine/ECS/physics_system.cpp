#include <ECS/physics_system.h>
#include <ECS/scene.h>

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
    }
    
    void PhysicsSystem::update(double delta)
    {
        
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
