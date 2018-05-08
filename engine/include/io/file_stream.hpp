#pragma once

#include <io/file.hpp>
#include <io/stream.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

// A Stream which writes and reads into a File object. Only guaranteed to work on OsFiles for now.
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
