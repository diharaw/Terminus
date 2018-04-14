#include <io/src/zip_directory.hpp>
#include <io/src/zip_file.hpp>
#include <memory/include/allocator.hpp>

#define ZIP_STATIC
#include <zip.h>

TE_BEGIN_TERMINUS_NAMESPACE

ZipDirectory::ZipDirectory()
{
	m_zip = nullptr;
}

ZipDirectory::ZipDirectory(FSNameBuffer name)
{
	int err;
	m_zip = zip_open(name.c_str(), 0, &err);
}

ZipDirectory::~ZipDirectory()
{
	zip_close((zip_t*)m_zip);
}

IFile* ZipDirectory::open_file(FSNameBuffer file, uint32_t mode, Allocator* allocator)
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

TE_END_TERMINUS_NAMESPACE
