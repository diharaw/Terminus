#pragma once

#include <ECS/component_types.h>
#include <ECS/entity.h>

namespace terminus
{
    // Forward Declarations
    class  Scene;
    struct Entity;
    
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
    };
}
