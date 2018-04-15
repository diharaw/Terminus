#pragma once

#include <io/include/filesystem.hpp>
#include <stl/include/murmur_hash.hpp>
#include <stl/include/packed_array.hpp>
#include <memory/src/buffer_pool_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class Package;

class FileSystem : public IFileSystem
{
public:
    FileSystem();
    ~FileSystem();
	virtual bool		 add_directory(const FSNameBuffer& path) override;
	virtual bool		 add_package(const FSNameBuffer& file) override;
	virtual bool		 remove_directory(const FSNameBuffer& path) override;
	virtual bool		 remove_package(const FSNameBuffer& file) override;
	virtual bool		 file_exists(const FSNameBuffer& file) override;
	virtual bool		 directory_exists(const FSNameBuffer& file) override;
	virtual bool		 create_directory(const FSNameBuffer& file) override;
	virtual IFile*       open_file(const FSNameBuffer& file, const uint32_t& mode, const bool& absolute = false) override;
	virtual void		 close_file(IFile* file) override;
	virtual FSNameBuffer file_extension(const FSNameBuffer& file) override;

private:
	IFile* open_file_internal(const FSNameBuffer& path, const uint32_t& mode);
    
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
