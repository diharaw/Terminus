#pragma once

#include <core/types.h>
#include <container/hash_map.h>

#define MAX_SCRIPT_FILES 20

namespace terminus
{
    class LuaScriptFileCache
    {
    private:
        HashMap<LuaScriptFile, MAX_SCRIPT_FILES> _script_file_map;
        
    public:
        LuaScriptFileCache();
        ~LuaScriptFileCache();
        LuaScriptFile* load(String file_name);
        bool is_loaded(String file_name);
        void unload(uint64_t file_name);
    };
} // namespace terminus
