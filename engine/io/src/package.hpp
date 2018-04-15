#include <io/include/file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class IAllocator;

class Package
{
public:
	Package();
	Package(const FSNameBuffer& name);
	~Package();
	IFile* open_file(const FSNameBuffer& file, const uint32_t& mode, IAllocator* allocator);

private:
	void* m_zip;
};

TE_END_TERMINUS_NAMESPACE
