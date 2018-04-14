#pragma once

#include <io/include/directory.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class DiskDirectory : public IDirectory
{
public:
	DiskDirectory();
	DiskDirectory(FSNameBuffer name);
	~DiskDirectory();
	IFile* open_file(FSNameBuffer file, uint32_t mode, Allocator* allocator) override;
};

TE_END_TERMINUS_NAMESPACE
