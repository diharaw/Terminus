#include <io/include/directory.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class ZipDirectory : public IDirectory
{
public:
	ZipDirectory();
	ZipDirectory(FSNameBuffer name);
	~ZipDirectory();
	IFile* open_file(FSNameBuffer file, uint32_t mode, Allocator* allocator);

private:
	void* m_zip;
};

TE_END_TERMINUS_NAMESPACE
