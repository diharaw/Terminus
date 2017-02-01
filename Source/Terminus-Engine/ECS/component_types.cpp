#include <ECS/component_types.h>
#include <ECS/scene.h>

namespace terminus
{
    void LuaScriptComponent::initialize()
    {
        _initialize = _object["initialize"];
        _update = _object["update"];
        _shutdown = _object["shutdown"];
        
        _object["_entity"] = _entity;
        _object["_scene"] = _scene;
        
        _initialize(_object, _entity);
    }
    
    void LuaScriptComponent::update(double dt)
    {
        _update(_object, dt);
    }
    
    void LuaScriptComponent::shutdown()
    {
        _shutdown(_object);
    }
}
