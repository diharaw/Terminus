#include <script/lua_script.h>
#include <gameplay/scene.h>

namespace terminus
{
    LuaScript::LuaScript()
    {
        
    }
    
    LuaScript::~LuaScript()
    {
        
    }
    
    void LuaScript::initialize(Scene* scene, Entity entity)
    {
        _initialize = _object["initialize"];
        _update = _object["update"];
        _shutdown = _object["shutdown"];
        
        _object["_entity"] = entity;
        _object["_scene"] = scene;
        
        _initialize(_object);
    }
    
    void LuaScript::update(double dt)
    {
        _update(_object, dt);
    }
    
    void LuaScript::shutdown()
    {
        _shutdown(_object);
    }
}
