#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <io/include/file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class IFileSystem
{
public:
    IFileSystem() {};
    virtual ~IFileSystem() {};
    virtual bool		 add_directory(const FSNameBuffer& path) = 0;
	virtual bool		 add_package(const FSNameBuffer& file) = 0;
	virtual bool		 remove_directory(const FSNameBuffer& path) = 0;
	virtual bool		 remove_package(const FSNameBuffer& file) = 0;
	virtual bool		 file_exists(const FSNameBuffer& file) = 0;
	virtual bool		 directory_exists(const FSNameBuffer& file) = 0;
	virtual bool		 create_directory(const FSNameBuffer& file) = 0;
    virtual IFile*       open_file(const FSNameBuffer& file, const uint32_t& mode, const bool& absolute = false) = 0;
	virtual void		 close_file(IFile* file) = 0;
    virtual FSNameBuffer file_extension(const FSNameBuffer& file) = 0;
};

TE_END_TERMINUS_NAMESPACE
