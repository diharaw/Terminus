#include <io/file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class IAllocator;

class Archive
{
public:
	Archive();
	Archive(const FSNameBuffer& name);
	~Archive();
	File* open_file(const FSNameBuffer& file, const uint32_t& mode, IAllocator* allocator);
	bool file_exists(const FSNameBuffer& file);

private:
	void* m_zip;
};

TE_END_TERMINUS_NAMESPACE
