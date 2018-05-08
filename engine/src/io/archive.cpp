#include <io/archive.hpp>
#include <io/zip_file.hpp>
#include <memory/allocator.hpp>

#define ZIP_STATIC
#include <zip.h>

TE_BEGIN_TERMINUS_NAMESPACE

Archive::Archive()
{
	m_zip = nullptr;
}

Archive::Archive(const FSNameBuffer& name)
{
	int err;
	m_zip = zip_open(name.c_str(), 0, &err);
}

Archive::~Archive()
{
	zip_close((zip_t*)m_zip);
}

File* Archive::open_file(const FSNameBuffer& file, const uint32_t& mode, IAllocator* allocator)
{
	if (allocator)
	{
		zip_file_t* zf = zip_fopen((zip_t*)m_zip, file.c_str(), 0);

		if (zf)
		{
			zip_stat_t stats;
			zip_stat((zip_t*)m_zip, file.c_str(), 0, &stats);
			ZipFile* zip_file = TE_NEW(allocator) ZipFile(zf, stats.size);

			return zip_file;
		}
		else
			return nullptr;
	}
	return nullptr;
}

bool Archive::file_exists(const FSNameBuffer& file)
{
	return zip_name_locate((zip_t*)m_zip, file.c_str(), 0) != -1;
}

TE_END_TERMINUS_NAMESPACE
