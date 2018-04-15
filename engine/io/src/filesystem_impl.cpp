#include <io/src/filesystem_impl.hpp>
#include <io/src/os_file.hpp>
#include <io/src/zip_file.hpp>
#include <io/src/package.hpp>
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

FileSystem::FileSystem()
{
	m_os_file_allocator.initialize(sizeof(OsFile), 8);
	m_zip_file_allocator.initialize(sizeof(ZipFile), 8);
	m_package_allocator.initialize(sizeof(Package), 8);
}

FileSystem::~FileSystem()
{
	for (int i = 0; i < m_directories.size(); i++)
		m_directories.remove(m_directories.array()[i].handle);

	for (int i = 0; i < m_packages.size(); i++)
	{
		TE_DELETE(m_packages.array()[i].package, &m_package_allocator);
		m_packages.remove(m_packages.array()[i].handle);
	}
}

bool FileSystem::add_directory(const FSNameBuffer& path)
{
	if (directory_exists(path))
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

bool FileSystem::add_package(const FSNameBuffer& file)
{
	if (file_exists(file))
	{
		PackageEntry entry;

		entry.name = file;
		entry.handle = m_packages.add();
		entry.package = TE_NEW(&m_package_allocator) Package(file);
		m_packages.set(entry.handle, entry);

		return true;
	}
	else
		return false;
}

bool FileSystem::remove_directory(const FSNameBuffer& file)
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

bool FileSystem::remove_package(const FSNameBuffer& file)
{
	for (int i = 0; i < m_packages.size(); i++)
	{
		if (m_packages.array()[i].name == file)
		{
			TE_DELETE(m_packages.array()[i].package, &m_package_allocator);
			m_packages.remove(m_packages.array()[i].handle);

			return true;
		}
	}

	return false;
}

IFile* FileSystem::open_file(const FSNameBuffer& file, const uint32_t& mode, const bool& absolute)
{
	if (absolute)
		return open_file_internal(file, mode);
	else
	{
		for (int i = 0; i < m_directories.size(); i++)
		{
			FSNameBuffer path = m_directories.array()[i].name + "/" + file;

			if (file_exists(path))
				return open_file_internal(path, mode);
		}

		for (int i = 0; i < m_packages.size(); i++)
		{
			IFile* zip_file = m_packages.array()[i].package->open_file(file, mode, &m_zip_file_allocator);
			
			if (zip_file)
				return zip_file;
		}

		return nullptr;
	}
}

void FileSystem::close_file(IFile* file)
{
	file->close();

	if (file->type() == TE_FILE_OS)
		TE_DELETE(file, &m_os_file_allocator);
	else
		TE_DELETE(file, &m_zip_file_allocator);
}

#if defined(TERMINUS_PLATFORM_WIN32)

bool FileSystem::file_exists(const FSNameBuffer& file)
{
	DWORD dwAttrib = GetFileAttributes(file.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileSystem::directory_exists(const FSNameBuffer& file)
{
	DWORD dwAttrib = GetFileAttributes(file.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
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

IFile* FileSystem::open_file_internal(const FSNameBuffer& path, const uint32_t& mode)
{
	std::string fileMode = "";

	if ((mode & TE_FS_READ) == TE_FS_READ)
		fileMode += "r";

	if ((mode & TE_FS_WRITE) == TE_FS_WRITE)
		fileMode += "w";

	if ((mode & TE_FS_BINARY) == TE_FS_BINARY)
		fileMode += "b";

	FILE* file = fopen(path.c_str(), fileMode.c_str());

	if (file)
		return TE_NEW(&m_os_file_allocator) OsFile((void*)file);
	else
		return nullptr;
}

TE_END_TERMINUS_NAMESPACE
