#pragma once

#include <io/include/filesystem.hpp>
#include <stl/include/murmur_hash.hpp>
#include <stl/include/packed_array.hpp>
#include <memory/src/buffer_pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class Package;

class FileSystemImpl : public FileSystem
{
public:
	FileSystemImpl();
    ~FileSystemImpl();
	virtual bool		 add_directory(const FSNameBuffer& path) override;
	virtual bool		 add_package(const FSNameBuffer& file) override;
	virtual bool		 remove_directory(const FSNameBuffer& path) override;
	virtual bool		 remove_package(const FSNameBuffer& file) override;
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

	struct PackageEntry
	{
		FSNameBuffer name;
		uint32_t	 handle;
		Package*	 package;
	};

	PackedArray<DirectoryEntry, 16>		m_directories;
	PackedArray<PackageEntry, 16>		m_packages;
    BufferPoolAllocator<TE_KILOBYTE(1)> m_zip_file_allocator;
	BufferPoolAllocator<TE_KILOBYTE(1)> m_os_file_allocator;
	BufferPoolAllocator<TE_KILOBYTE(1)> m_package_allocator;
};

TE_END_TERMINUS_NAMESPACE
