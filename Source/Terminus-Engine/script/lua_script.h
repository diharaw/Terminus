#pragma once

#include <types.h>

namespace terminus
{
    class Scene;
    struct Entity;
    
    class LuaScript
    {
    public:
        LuaFunction _initialize;
        LuaFunction _update;
        LuaFunction _shutdown;
        LuaObject	_object;
        String      _instance_name;
        
    public:
        LuaScript();
        ~LuaScript();
        
        void initialize(Scene* scene, Entity entity);
        void update(double dt);
        void shutdown();
    };
}
