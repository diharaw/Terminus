#include <iostream>
#include <string>
#include <Utility/runtime_compile.h>
#include <Core/config.h>

namespace runtime_compile
{
    bool compile(const char* input_path, const char* source, const char* output_path, const char* include_paths, const char* flags)
    {
        if(output_path && input_path && source)
        {
            std::string _source = std::string(source);
            std::string _input_path = std::string(input_path);
            std::string _output_path = std::string(output_path);
#if defined(TERMINUS_PLATFORM_WIN32)
            _output_path += ".dll";
#elif defined(TERMINUS_PLATFORM_MACOS)
            _output_path += ".dylib";
#else
            _output_path += ".so;
#endif
            std::string _include_paths = std::string(include_paths);
            std::string _flags = std::string(flags);
            
            std::string command = "cd ";
            command += _input_path;
            
#if defined(__clang__)
            command += "; ";
            command += "clang++ -shared -undefined dynamic_lookup -o ";
            command += _output_path;
            command += " ";
            command += source;
            command += " ";
            command += _include_paths;
            command += " ";
            command += _flags;
#elif defined(__GNUC__)
            command += "; ";
            command += "g++ -Wall -shared -fPIC -o "
            command += _output_path;
            command += " ";
            command += _source;
            command += " ";
            command += _include_paths;
            command += " ";
            command += _flags;
#elif defined(_MSC_VER)
            command += " && ";
            command += " vcvars32 && ";
            command += "cl.exe /D_USRDLL /D_WINDLL ";
            command += _source;
            command += " /MT /link /DLL /OUT:";
            command += _output_path;
#endif
            int status = system(command.c_str());
            
            if(status == 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
}
