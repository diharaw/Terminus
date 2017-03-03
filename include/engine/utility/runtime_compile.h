#pragma once

#include <vector>
#include <string>

namespace terminus
{
	namespace runtime_compile
	{
		extern bool compile(const char* input_path, const char* source, const char* output_path, std::vector<std::string> include_paths, const char* flags);
	}
}
