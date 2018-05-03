#pragma once

#include <io/include/filesystem.hpp>
#include <io/src/os_file.hpp>
#include <io/src/zip_file.hpp>
#include <io/src/archive.hpp>
#include <stl/include/murmur_hash.hpp>
#include <stl/include/packed_array.hpp>
#include <memory/src/buffer_pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class Archive;

class FileSystemImpl : public FileSystem
{
public:
	FileSystemImpl();
    ~FileSystemImpl();
	virtual bool		 add_search_directory(const FSNameBuffer& path) override;
	virtual bool		 add_search_archive(const FSNameBuffer& file) override;
	virtual bool		 remove_search_directory(const FSNameBuffer& path) override;
	virtual bool		 remove_search_archive(const FSNameBuffer& file) override;
	virtual bool		 file_exists(const FSNameBuffer& file, bool absolute = false) override;
	virtual bool		 directory_exists(const FSNameBuffer& file, bool absolute = false) override;
	virtual bool		 create_directory(const FSNameBuffer& file) override;
	virtual File*        open_file(const FSNameBuffer& file, const uint32_t& mode) override;
	virtual File*        open_file_ex(const FSNameBuffer& file, const uint32_t& mode) override;
	virtual void		 close_file(File* file) override;
	virtual FSNameBuffer file_extension(const FSNameBuffer& file) override;

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
