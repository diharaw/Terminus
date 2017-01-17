#include <iostream>
#include <string>
#include "runtime_compile.h"

namespace runtime_compiler
{
    bool compile(const char* input_path, const char* source, const char* lib_name, const char* output_path)
    {
        std::string _source = std::string(source);
        std::string _lib_name = std::string(lib_name);
        std::string _input_path = std::string(input_path);
        std::string _output_path;
        
        if(output_path)
            _output_path = std::string(output_path);
        else
            _output_path = "";
        
        std::string command = "cd ";
        command += _input_path;
        
#if defined(__clang__)
		command += "; ";
        command += "clang++ -shared -undefined dynamic_lookup -o ";
        command += _lib_name;
        command += " ";
        command += source;
#elif defined(__GNUC__)
		command += "; ";
        command += "g++ -Wall -shared -fPIC -o "
        command += _lib_name;
        command += " ";
        command += _source;
#elif defined(_MSC_VER)
		command += " && ";
		command += " vcvars32 && ";
        command += "cl.exe /D_USRDLL /D_WINDLL ";
        command += _source;
        command += " /MT /link /DLL /OUT:";
        command += _lib_name;
#endif
        int status = system(command.c_str());
        
        if(status == 0)
            return true;
        else
            return false;
    }
}
