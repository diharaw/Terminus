#pragma once

#include <stdio.h>
#include <climits>
#include "macros.hpp"

PROTOGFX_BEGIN_NAMESPACE

class OsFile
{
public:
	const size_t END = ULLONG_MAX;

	OsFile(void* handle);
	~OsFile();
	size_t size();
	void read(size_t size, void* buffer);
	void seek(size_t offset);

private:
	size_t _start;
	void* _handle;
};

enum class FileMode
{
	READ = 0,
	READ_TEXT = 1,
	WRITE = 2,
	READ_WRITE = 3
};

namespace filesystem
{
	extern OsFile* open_file(const char* file, FileMode mode);
	extern void close_file(OsFile* file);
}

PROTOGFX_END_NAMESPACE