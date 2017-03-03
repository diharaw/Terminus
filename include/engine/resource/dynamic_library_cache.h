#pragma once

#include <types.h>
#include <unordered_map>
#include <vector>
#include <Utility/dynamic_library.h>

namespace terminus
{
    class DynamicLibraryCache
    {
    private:
        std::unordered_map<std::string, dynamic_library::LibHandle> _library_map;
        
    public:
        DynamicLibraryCache();
        ~DynamicLibraryCache();
        dynamic_library::LibHandle* load(String file_name);
        void shutdown();
    };
} // namespace terminus
