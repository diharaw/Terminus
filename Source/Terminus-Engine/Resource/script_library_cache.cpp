#include <Resource/script_library_cache.h>
#include <IO/logger.h>
#include <IO/filesystem.h>

namespace terminus
{
    ScriptLibraryCache::ScriptLibraryCache()
    {
        
    }
    
    ScriptLibraryCache::~ScriptLibraryCache()
    {
        
    }
    
    void ScriptLibraryCache::shutdown()
    {
        for(auto it : _library_map)
        {
            if(dynamic_library::valid_handle(it.second._handle))
            {
                for(auto script : it.second._instances)
                {
                    if(script)
                    {
                        T_SAFE_DELETE(script);
                    }
                }
                
                dynamic_library::close(it.second._handle);
            }
        }
    }
    
    CppScript* ScriptLibraryCache::create_script_from_library(String script_name, String class_name)
    {
        if (_library_map.find(script_name) == _library_map.end())
        {
            ScriptLibrary lib;
            lib._class_name = class_name;
            lib._handle = dynamic_library::open(script_name.c_str());
            
            if(dynamic_library::valid_handle(lib._handle))
            {
                _library_map[script_name] = lib;
            }
            else
            {
                T_LOG_ERROR("Creating Cpp Script failed : Failed to load library " + script_name);
                return nullptr;
            }
        }
        
        ScriptLibrary& library = _library_map[script_name];
        String factory_name = "Create";
        factory_name += class_name;
        CppScript* cpp_script = dynamic_library::create_instance_from_factory<CppScript>(factory_name.c_str(), library._handle);
        library._instances.push_back(cpp_script);
        
        return cpp_script;
    }
}
