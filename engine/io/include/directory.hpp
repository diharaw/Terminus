#pragma once

#include <stdio.h>
#include <climits>
#include <core/include/terminus_macros.hpp>
#include <memory/include/memory_macros.hpp>
#include <stl/include/string_buffer.hpp>
#include <io/include/io_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE



class IFile;
class IAllocator;

class IDirectory
{
public:
	IDirectory() {}
	virtual ~IDirectory() {}
	virtual IFile* open_file(FSNameBuffer file, uint32_t mode, IAllocator* allocator) = 0;

protected:
	FSNameBuffer m_name;
};

TE_END_TERMINUS_NAMESPACE
