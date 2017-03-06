#include <resource/cpp_script_component_factory.h>
#include <gameplay/component_types.h>
#include <core/context.h>

namespace terminus
{
    namespace cpp_script_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            ScriptEngine& script_engine = context::get_script_engine();
            
            CppScriptComponent& component = scene->attach_cpp_script_component(entity);
            
            component._class_name = String(json["class_name"].GetString());
            component._file_name = String(json["script_file"].GetString());
            component._scene = scene;
            component._entity = entity;
            component._script = script_engine.create_cpp_script(component._file_name, component._class_name);
            //component._script->_entity = entity;
            component._script->_scene = scene;
        }        
    }
}
