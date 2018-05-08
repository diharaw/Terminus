#include <io/filesystem.hpp>
#include <core/engine_core.hpp>
#include <string>
#include <zip.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#include <direct.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#endif

TE_BEGIN_TERMINUS_NAMESPACE

FileSystem::FileSystem() {}

FileSystem::~FileSystem()
{
	for (int i = 0; i < m_directories.size(); i++)
		m_directories.remove(m_directories.array()[i].handle);

	for (int i = 0; i < m_archives.size(); i++)
	{
		TE_DELETE(m_archives.array()[i].archive, &m_archive_allocator);
		m_archives.remove(m_archives.array()[i].handle);
	}
}

bool FileSystem::add_search_directory(const FSNameBuffer& path)
{
	if (directory_exists(path, true))
	{
		DirectoryEntry entry;

		entry.name = path;
		entry.handle = m_directories.add();
		m_directories.set(entry.handle, entry);

		return true;
	}
	else
		return false;
}

bool FileSystem::add_search_archive(const FSNameBuffer& file)
{
	if (file_exists(file, true))
	{
		ArchiveEntry entry;

		entry.name = file;
		entry.handle = m_archives.add();
		entry.archive = TE_NEW(&m_archive_allocator) Archive(file);
		m_archives.set(entry.handle, entry);

		return true;
	}
	else
		return false;
}

bool FileSystem::remove_search_directory(const FSNameBuffer& file)
{
	for (int i = 0; i < m_directories.size(); i++)
	{
		if (m_directories.array()[i].name == file)
		{
			m_directories.remove(m_directories.array()[i].handle);
			return true;
		}
	}

	return false;
}

bool FileSystem::remove_search_archive(const FSNameBuffer& file)
{
	for (int i = 0; i < m_archives.size(); i++)
	{
		if (m_archives.array()[i].name == file)
		{
			TE_DELETE(m_archives.array()[i].archive, &m_archive_allocator);
			m_archives.remove(m_archives.array()[i].handle);

			return true;
		}
	}

	return false;
}

File* FileSystem::open_file(const FSNameBuffer& file, const uint32_t& mode)
{
	StringBuffer<4> fileMode = "";

	if ((mode & TE_FS_READ) == TE_FS_READ)
		fileMode += "r";

	if ((mode & TE_FS_WRITE) == TE_FS_WRITE)
		fileMode += "w";

	if ((mode & TE_FS_BINARY) == TE_FS_BINARY)
		fileMode += "b";

	FILE* os_file = fopen(file.c_str(), fileMode.c_str());

	if (os_file)
		return TE_NEW(&m_os_file_allocator) OsFile((void*)os_file);
	else
		return nullptr;
}

File* FileSystem::open_file_ex(const FSNameBuffer& file, const uint32_t& mode)
{
	// File writing is not allowed in extended mode.
	assert((mode & TE_FS_WRITE) != TE_FS_WRITE);

	for (int i = 0; i < m_directories.size(); i++)
	{
		FSNameBuffer path = m_directories.array()[i].name + "/" + file;

		if (file_exists(path, true))
			return open_file(path, mode);
	}

	for (int i = 0; i < m_archives.size(); i++)
	{
		File* zip_file = m_archives.array()[i].archive->open_file(file, mode, &m_zip_file_allocator);

		if (zip_file)
			return zip_file;
	}

	return nullptr;
}

void FileSystem::close_file(File* file)
{
	file->close();

	if (file->type() == TE_FILE_OS)
		TE_DELETE(file, &m_os_file_allocator);
	else
		TE_DELETE(file, &m_zip_file_allocator);
}

#if defined(TERMINUS_PLATFORM_WIN32)

bool FileSystem::file_exists(const FSNameBuffer& file, bool absolute)
{
	if (absolute)
		return file_exists_internal(file);
	else
	{
		for (int i = 0; i < m_directories.size(); i++)
		{
			FSNameBuffer path = m_directories.array()[i].name + "/" + file;
			bool exists = file_exists_internal(path);

			if (exists)
				return exists;
		}

		for (int i = 0; i < m_archives.size(); i++)
		{
			bool exists = m_archives.array()[i].archive->file_exists(file);

			if (exists)
				return exists;
		}

		return false;
	}
}

bool FileSystem::directory_exists(const FSNameBuffer& file, bool absolute)
{
	if (absolute)
		return directory_exists_internal(file);
	else
	{
		for (int i = 0; i < m_directories.size(); i++)
		{
			FSNameBuffer path = m_directories.array()[i].name + "/" + file;
			bool exists = directory_exists_internal(path);

			if (exists)
				return exists;
		}

		return false;
	}
}

bool FileSystem::create_directory(const FSNameBuffer& path)
{
	FSNameBuffer str_path = path;;

	int ret = _mkdir(path.c_str());

	if (ret == 0)
		return true;

	switch (errno)
	{
	case ENOENT:
	{
		int pos = static_cast<int>(str_path.find_last('/'));
		if (pos == FSNameBuffer::END)
			pos = str_path.find_last('\\');
		if (pos == FSNameBuffer::END)
			return false;
		if (!create_directory(str_path.substring(0, pos).c_str()))
			return false;
	}

	return 0 == _mkdir(path.c_str());

	case EEXIST:
		return directory_exists(path);

	default:
		return false;
	}
}

#else

bool FileSystem::file_exists(const FSNameBuffer& file)
{
	if (access(path, F_OK) != -1)
		return true;
	else
		return false;
}

bool FileSystem::directory_exists(const FSNameBuffer& file)
{
	if (path == NULL)
		return false;

	DIR *dir;
	bool exists = false;

	dir = opendir(path);

	if (dir != NULL)
	{
		exists = true;
		closedir(dir);
	}

	return exists;
}

bool FileSystem::create_directory(const FSNameBuffer& file)
{
	FSNameBuffer str_path = path;;

	mode_t mode = 0755;
	int ret = mkdir(path, mode);

	if (ret == 0)
		return true;

	switch (errno)
	{
	case ENOENT:
	{
		int pos = static_cast<int>(str_path.find_last('/'));
		if (pos == FSNameBuffer::END)
			return false;
		if (!create_directory(str_path.substring(0, pos).c_str()))
			return false;
	}

	return 0 == mkdir(path, mode);

	case EEXIST:
		return directory_exists(path);

	default:
		return false;
	}
}

#endif

FSNameBuffer FileSystem::file_extension(const FSNameBuffer& file)
{
    size_t pos = file.find_last('.');
    
    if (pos == FSNameBuffer::END)
        return "";
    else
        return file.substring(pos + 1, FSNameBuffer::END);
}

bool FileSystem::file_exists_internal(const FSNameBuffer& path)
{
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileSystem::directory_exists_internal(const FSNameBuffer& path)
{
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

TE_END_TERMINUS_NAMESPACE
