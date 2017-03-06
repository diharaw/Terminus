#pragma once

#include <gameplay/component_types.h>
#include <gameplay/entity.h>

namespace terminus
{
    // Forward Declarations
    class  Scene;
    struct Entity;
    
    struct RigidBodyTransform
    {
        ID rigid_body;
        ID transform;
    };
    
    class PhysicsSystem
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem();
        void initialize(Scene* scene);
        void update(double delta);
        void shutdown();
        void on_entity_created(Entity entity);
        void on_entity_destroyed(Entity entity);
        
    private:
        Scene* _scene;
        PackedArray<RigidBodyTransform, MAX_ENTITIES> _pairs;
        ID _entities[MAX_ENTITIES];
    };
}
