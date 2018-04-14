#pragma once

#include <io/src/zip_file.hpp>
#define ZIP_STATIC
#include <zip.h>
#include <assert.h>

TE_BEGIN_TERMINUS_NAMESPACE

ZipFile::ZipFile(void* handle, size_t size) : m_size(size), m_handle(handle)
{

}

ZipFile::~ZipFile()
{
	
}

size_t ZipFile::size()
{
	return m_size;
}

size_t ZipFile::read(void* buffer, size_t size, size_t count)
{
	return zip_fread((zip_file_t*)m_handle, buffer, size);
}

size_t ZipFile::write(void* data, size_t size, size_t count)
{
	// NOT ALLOWED.
	assert(false);
	return 0;
}

void ZipFile::seek(size_t offset)
{
	// NOT ALLOWED.
	assert(false);
}

void* ZipFile::handle()
{
	return m_handle;
}

void ZipFile::close()
{
    zip_fclose((zip_file_t*)m_handle);
}

TE_END_TERMINUS_NAMESPACE
