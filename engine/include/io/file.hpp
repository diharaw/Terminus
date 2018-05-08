#pragma once

#include <stdio.h>
#include <climits>
#include <core/terminus_types.hpp>
#include <core/terminus_macros.hpp>
#include <stl/string_buffer.hpp>
#include <io/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

using FSNameBuffer = StringBuffer<TE_MAX_FILENAME_SIZE>;

enum FileMode
{
	TE_FS_READ	 = 1,
	TE_FS_WRITE  = 2,
	TE_FS_BINARY = 4
};

enum FileType
{
	TE_FILE_OS = 0,
	TE_FILE_ZIP  = 1
};

class File
{
public:
	const size_t END = ULLONG_MAX;

	File() {}
	virtual ~File() {}
	virtual size_t size() = 0;
	virtual size_t read(void* buffer, const size_t& size, const size_t& count) = 0;
	virtual size_t write(void* data, const size_t& size, const size_t& count) = 0;
	virtual FileType type() = 0;
	virtual void seek(const size_t& offset) = 0;
	virtual void* handle() = 0;
    virtual void close() = 0;
};

TE_END_TERMINUS_NAMESPACE
