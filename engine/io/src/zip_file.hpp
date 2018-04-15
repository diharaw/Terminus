#pragma once

#include <io/include/file.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class ZipFile : public IFile
{
public:
	ZipFile(void* handle, size_t size);
	~ZipFile();
	virtual size_t size() override;
	virtual FileType type() override;
	virtual size_t read(void* buffer, const size_t& size, const size_t& count) override;
	virtual size_t write(void* data, const size_t& size, const size_t& count) override;
	virtual void seek(const size_t& offset) override;
	void* handle() override;
    void close() override;

private:
	size_t		m_start;
	size_t		m_size;
	void*		m_handle;
};

TE_END_TERMINUS_NAMESPACE
