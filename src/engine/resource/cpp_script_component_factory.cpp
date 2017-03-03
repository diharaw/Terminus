#include <Resource/cpp_script_component_factory.h>
#include <ECS/component_types.h>
#include <Core/context.h>

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
        
#if defined(TERMINUS_WITH_EDITOR)
        void create_from_desc(CppScriptDesc* desc, Entity& entity, Scene* scene)
        {
            ScriptEngine& script_engine = context::get_script_engine();
            
            CppScriptComponent& component = scene->attach_cpp_script_component(entity);
            
            component._class_name = String(desc->_class_name.c_str());
            component._file_name = String(desc->_script_name.c_str());
            component._scene = scene;
            component._entity = entity;
            component._script = script_engine.create_cpp_script(component._file_name, component._class_name);
            //component._script->_entity = entity;
            component._script->_scene = scene;
        }
        
        void create(JsonValue& json, EditorEntity& entity)
        {
            CppScriptDesc* desc = new CppScriptDesc();
            desc->_script_name = json["script_file"].GetString();
            desc->_class_name = json["class_name"].GetString();
            entity.add_component_desc(EditorComponentType::CPP_SCRIPT, desc);
        }
#endif
    }
}
