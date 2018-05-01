#pragma once

#include <io/include/file.hpp>
#include <io/include/stream.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class FileStream : public IStream
{
public:
    FileStream(File* file);
    ~FileStream();
    void seek(const size_t& offset) override;
    void write(void* src, const size_t& size) override;
    void read(void* dst, const size_t& size) override;
    void reset() override;
	size_t size() override;
  
private:
    File* m_file;
};

TE_END_TERMINUS_NAMESPACE
