#include <io/src/disk_directory.hpp>
#include <io/src/os_file.hpp>
#include <memory/include/allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

DiskDirectory::DiskDirectory()
{
	m_name = "";
}

DiskDirectory::DiskDirectory(FSNameBuffer name)
{
	m_name = name;
}

DiskDirectory::~DiskDirectory()
{

}

IFile* DiskDirectory::open_file(FSNameBuffer file, uint32_t mode, Allocator* allocator)
{
	if (allocator)
	{
		std::string fileMode = "";

        if((mode & TE_FS_READ) == TE_FS_READ)
            fileMode += "r";
        
        if((mode & TE_FS_WRITE) == TE_FS_WRITE)
            fileMode += "w";
        
        if((mode & TE_FS_BINARY) == TE_FS_BINARY)
            fileMode += "b";
        
		FSNameBuffer path = m_name;

		if (m_name != "")
			path += "/";

		path += file;
		 
		FILE* _file = fopen(path.c_str(), fileMode.c_str());

		if (_file)
			return TE_NEW(allocator) OsFile((void*)_file);
		else
			return nullptr;
	}
	else
		return nullptr;
}

TE_END_TERMINUS_NAMESPACE
