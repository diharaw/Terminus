#pragma once

#include <types.h>
#include <script/cpp_script.h>
#include <unordered_map>
#include <vector>
#include <Utility/dynamic_library.h>

namespace terminus
{
    struct ScriptLibrary
    {
        String                     _class_name;
        dynamic_library::LibHandle _handle;
        std::vector<CppScript*>    _instances;
    };
    
    class ScriptLibraryCache
    {
    private:
        std::unordered_map<std::string, ScriptLibrary> _library_map;
        
    public:
        ScriptLibraryCache();
        ~ScriptLibraryCache();
        void shutdown();
        
        CppScript* create_script_from_library(String script_name, String class_name); // used as class name as well.
    };
} // namespace terminus
