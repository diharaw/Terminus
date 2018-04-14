#pragma once

#include <io/include/file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class OsFile : public IFile
{
public:
	OsFile(void* handle);
	~OsFile();
	size_t size() override;
	size_t read(void* buffer, size_t size, size_t count) override;
	size_t write(void* data, size_t size, size_t count) override;
	void seek(size_t offset) override;
	void* handle() override;
    void close() override;

private:
	size_t m_start;
	void*  m_handle;
};

TE_END_TERMINUS_NAMESPACE
