#include <io/file_stream.hpp>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

FileStream::FileStream(File* file) : m_file(file)
{

}

FileStream::~FileStream()
{
    
}

void FileStream::seek(const size_t& offset)
{
	m_pointer += offset;
	m_file->seek(m_pointer);
}

void FileStream::write(void* src, const size_t& size)
{
	m_file->write(src, size, 1);

	if (m_auto_move_write)
		seek(size);
}

void FileStream::read(void* dst, const size_t& size)
{
	m_file->read(dst, size, 1);

	if (m_auto_move_read)
		seek(size);
}

void FileStream::reset()
{
	m_pointer = 0;
}

size_t FileStream::size()
{
	return m_file->size();
}

TE_END_TERMINUS_NAMESPACE
