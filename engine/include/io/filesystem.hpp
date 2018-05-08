#pragma once

#include <core/terminus_macros.hpp>
#include <io/io_macros.hpp>
#include <io/file.hpp>
#include <io/os_file.hpp>
#include <io/zip_file.hpp>
#include <io/archive.hpp>
#include <stl/murmur_hash.hpp>
#include <stl/packed_array.hpp>
#include <memory/buffer_pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class Archive;

class FileSystem
{
public:
	FileSystem();
	~FileSystem();
	bool		 add_search_directory(const FSNameBuffer& path);
	bool		 add_search_archive(const FSNameBuffer& file);
	bool		 remove_search_directory(const FSNameBuffer& path);
	bool		 remove_search_archive(const FSNameBuffer& file);
	bool		 file_exists(const FSNameBuffer& file, bool absolute = false);
	bool		 directory_exists(const FSNameBuffer& file, bool absolute = false);
	bool		 create_directory(const FSNameBuffer& file);
	File*        open_file(const FSNameBuffer& file, const uint32_t& mode);
	File*        open_file_ex(const FSNameBuffer& file, const uint32_t& mode);
	void		 close_file(File* file);
	FSNameBuffer file_extension(const FSNameBuffer& file);

private:
	bool file_exists_internal(const FSNameBuffer& path);
	bool directory_exists_internal(const FSNameBuffer& path);

private:
	struct DirectoryEntry
	{
		FSNameBuffer name;
		uint32_t	 handle;
	};

	struct ArchiveEntry
	{
		FSNameBuffer name;
		uint32_t	 handle;
		Archive*	 archive;
	};

	PackedArray<DirectoryEntry, 16>		m_directories;
	PackedArray<ArchiveEntry, 16>		m_archives;
	BufferPoolAllocator<TE_KILOBYTE(1), sizeof(ZipFile)> m_zip_file_allocator;
	BufferPoolAllocator<TE_KILOBYTE(1), sizeof(OsFile)>  m_os_file_allocator;
	BufferPoolAllocator<TE_KILOBYTE(1), sizeof(Archive)> m_archive_allocator;
};

TE_END_TERMINUS_NAMESPACE
