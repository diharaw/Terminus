#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "System.h"

namespace Terminus { namespace ECS {
    
    class RenderSystem : public ISystem
    {
    public:
        RenderSystem();
        ~RenderSystem();
        virtual void Initialize();
        virtual void Update(double delta);
        virtual void Shutdown();
        void OnEntityCreated(Entity entity);
        void OnEntityDestroyed(Entity entity);
    };
    
} }

#endif
