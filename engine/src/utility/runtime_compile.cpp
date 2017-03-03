#include <Utility/runtime_compile.h>
#include <Utility/process_dispatch.h>
#include <Core/config.h>
#include <IO/logger.h>

#include <iostream>
#include <string>
#include <cstdio>

namespace terminus
{
	namespace runtime_compile
	{
		bool compile(const char* input_path, const char* source, const char* output_path, std::vector<std::string> include_paths, const char* flags)
		{
			if (output_path && input_path && source)
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

				std::string _include_paths = "";

				for (std::string path : include_paths)
				{
#if defined(TERMINUS_COMPILER_CLANG) || (TERMINUS_COMPILER_GCC)
					_include_paths += " -I \"";
#else
					_include_paths += " /I \"";
#endif
					_include_paths += path;
					_include_paths += "\"";
				}

				std::string _flags = std::string(flags);

#if defined(TERMINUS_COMPILER_CLANG)
				std::string command = "cd ";
				command += _input_path;
				command += "; ";
				command += "clang++ -shared -undefined dynamic_lookup -o ";
				command += _output_path;
				command += " ";
				command += source;
				command += " ";
				command += _include_paths;
				command += " ";
				command += _flags;
#elif defined(TERMINUS_COMPILER_GCC)
				std::string command = "cd ";
				command += _input_path;
				command += "; ";
				command += "g++ -Wall -shared -fPIC -o "
					command += _output_path;
				command += " ";
				command += _source;
				command += " ";
				command += _include_paths;
				command += " ";
				command += _flags;
#elif defined(TERMINUS_COMPILER_MSVC)
				std::string command = "cmd.exe /C cd ";
				command += _input_path;
				command += " && ";
				command += " vcvarsall amd64 && ";
				command += "cl.exe /D_USRDLL /D_WINDLL ";
				command += _source;
				// TODO: take debug/release into consideration
				command += " /MDd /link /DLL /OUT:";
				command += _output_path;
#endif

				process_dispatch::ProcessHandle handle = process_dispatch::create(command.c_str());
				process_dispatch::close(handle);

				if (handle.exit_code == process_dispatch::ProcessExitCode::SUCCESS)
					return true;
				else
					return false;
			}
			else
			{
				T_LOG_ERROR("Output path, input path or Source unspecified.");
				return false;
			}
		}
	}

}