#include <io/src/os_file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

OsFile::OsFile(void* handle) : m_handle(handle)
{
	
}

OsFile::~OsFile()
{
	fclose((FILE*)m_handle);
}

size_t OsFile::size()
{
	fseek((FILE*)m_handle, 0, SEEK_END);
	size_t size = ftell((FILE*)m_handle);
	fseek((FILE*)m_handle, 0, SEEK_SET);
	return size;
}

size_t OsFile::read(void* buffer, size_t size, size_t count)
{
	return fread(buffer, size, count, (FILE*)m_handle);
}

size_t OsFile::write(void* data, size_t size, size_t count)
{
	return fwrite(data, size, count, (FILE*)m_handle);
}

void OsFile::seek(size_t offset)
{
	if (offset == END)
		fseek((FILE*)m_handle, 0, SEEK_END);
	else
		fseek((FILE*)m_handle, offset, SEEK_SET);
}

void* OsFile::handle()
{
	return m_handle;
}

void OsFile::close()
{
    if(m_handle)
        fclose((FILE*)m_handle);
}

TE_END_TERMINUS_NAMESPACE
