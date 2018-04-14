#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <io/include/directory.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class IFileSystem
{
public:
    IFileSystem() {};
    virtual ~IFileSystem() {};
    virtual void         vfs_mount_dir(FSNameBuffer dir, FSNameBuffer point) = 0;
    virtual IFile*       vfs_open_file(FSNameBuffer file, uint32_t mode) = 0;
    virtual FSNameBuffer file_extension(FSNameBuffer file) = 0;
};

TE_END_TERMINUS_NAMESPACE
