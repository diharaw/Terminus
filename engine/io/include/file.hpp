#pragma once

#include <stdio.h>
#include <climits>
#include <core/include/terminus_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class IFile
{
public:
	const size_t END = ULLONG_MAX;

	IFile() {}
	virtual ~IFile() {}
	virtual size_t size() = 0;
	virtual size_t read(void* buffer, size_t size, size_t count) = 0;
	virtual size_t write(void* data, size_t size, size_t count) = 0;
	virtual void seek(size_t offset) = 0;
	virtual void* handle() = 0;
    virtual void close() = 0;
};

TE_END_TERMINUS_NAMESPACE
