#include <resource/dynamic_library_cache.h>
#include <io/logger.h>
#include <io/filesystem.h>

namespace terminus
{
    DynamicLibraryCache::DynamicLibraryCache()
    {
        
    }
    
    DynamicLibraryCache::~DynamicLibraryCache()
    {
        
    }
    
    void DynamicLibraryCache::shutdown()
    {
        for(auto it : _library_map)
        {
            if(dynamic_library::valid_handle(it.second))
                dynamic_library::close(it.second);
        }
    }
    
    dynamic_library::LibHandle* DynamicLibraryCache::load(String file_name)
    {
        if (_library_map.find(file_name) == _library_map.end())
        {
            dynamic_library::LibHandle lib = dynamic_library::open(file_name.c_str());
            
            if(dynamic_library::valid_handle(lib))
            {
                _library_map[file_name] = lib;
                return &_library_map[file_name];
            }
            else
            {
                T_LOG_ERROR("Failed to load dynamic library " + file_name);
                return nullptr;
            }
        }
        else
            return &_library_map[file_name];
    }
}
