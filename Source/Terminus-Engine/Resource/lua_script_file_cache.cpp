#include <Resource/lua_script_file_cache.h>
#include <Utility/murmur_hash.h>
#include <IO/filesystem.h>

namespace terminus
{
    LuaScriptFileCache::LuaScriptFileCache()
    {
        filesystem::add_directory("assets/script/lua");
    }
    
    LuaScriptFileCache::~LuaScriptFileCache()
    {
        
    }
    
    LuaScriptFile* LuaScriptFileCache::load(String file_name)
    {
        uint64_t hash = HASH(file_name.c_str());
        
        if(_script_file_map.has(hash))
            return _script_file_map.get_ref(hash);
        else
        {
            FileHandle handle = filesystem::read_file(file_name, true);
            
            if(handle.buffer)
            {
                LuaScriptFile script_file = handle.buffer;
                _script_file_map.set(hash, script_file);
                
                filesystem::destroy_handle(handle);
                
                return _script_file_map.get_ref(hash);
            }
            else
                return nullptr;
        }
    }
    
    bool LuaScriptFileCache::is_loaded(String file_name)
    {
        uint64_t hash = HASH(file_name.c_str());
        return _script_file_map.has(hash);
    }
    
    void LuaScriptFileCache::unload(uint64_t file_name)
    {
        if(_script_file_map.has(file_name))
            _script_file_map.remove(file_name);
    }
}
