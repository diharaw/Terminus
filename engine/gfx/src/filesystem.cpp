#include "filesystem.hpp"
#include <string>

PROTOGFX_BEGIN_NAMESPACE

OsFile::OsFile(void* handle)
{
	_handle = handle;
}

OsFile::~OsFile()
{
	fclose((FILE*)_handle);
}

size_t OsFile::size()
{
	fseek((FILE*)_handle, 0, SEEK_END);
	size_t size = ftell((FILE*)_handle);
	fseek((FILE*)_handle, 0, SEEK_SET);
	return size;
}

void OsFile::read(size_t size, void* buffer)
{
	fread(buffer, size, 1, (FILE*)_handle);
}

void OsFile::seek(size_t offset)
{
	if (offset == END)
		fseek((FILE*)_handle, 0, SEEK_END);
	else
		fseek((FILE*)_handle, offset, SEEK_SET);
}

namespace filesystem
{
	OsFile* open_file(const char* file, FileMode mode)
	{
		std::string fileMode;
        
		switch (mode)
		{
			case FileMode::WRITE:
			{
				fileMode = "w";
				break;
			}
			case FileMode::READ:
			{
				fileMode = "rb";
				break;
			}
			case FileMode::READ_TEXT:
			{
				fileMode = "rb";
				break;
			}
			case FileMode::READ_WRITE:
			{
				fileMode = "rw";
				break;
			}
		}

		FILE* _file = fopen(file, fileMode.c_str());

		if (!_file)
			return nullptr;
		else
			return new OsFile((void*)_file);
	}

	void close_file(OsFile* file)
	{
		if (file)
			delete file;
	}
}

PROTOGFX_END_NAMESPACE
