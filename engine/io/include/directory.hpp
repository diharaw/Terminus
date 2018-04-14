#pragma once

#include <stdio.h>
#include <climits>
#include <core/include/terminus_macros.hpp>
#include <memory/include/memory_macros.hpp>
#include <stl/include/string_buffer.hpp>
#include <io/include/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

using FSNameBuffer = StringBuffer<TE_MAX_FILENAME_SIZE>;

enum FileMode
{
	TE_FS_READ       = 1,
	TE_FS_WRITE      = 2,
    TE_FS_TEXT       = 4,
    TE_FS_BINARY     = 8
};

class IFile;
class Allocator;

class IDirectory
{
public:
	IDirectory() {}
	virtual ~IDirectory() {}
	virtual IFile* open_file(FSNameBuffer file, uint32_t mode, Allocator* allocator) = 0;

protected:
	FSNameBuffer m_name;
};

TE_END_TERMINUS_NAMESPACE
