#include <Resource/cpp_script_component_factory.h>
#include <Core/context.h>

namespace terminus
{
    namespace cpp_script_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            ScriptLibraryCache& cache = context::get_script_library_cache();
            
            CppScriptComponent& component = scene->attach_cpp_script_component(entity);
            
            component._class_name = String(json["class_name"].GetString());
            component._file_name = String(json["script_file"].GetString());
            component._scene = scene;
            component._entity = entity;
            component._script = cache.create_script_from_library(component._file_name, component._class_name);
            component._script->_entity = entity;
            component._script->_scene = scene;
        }
    }
}
