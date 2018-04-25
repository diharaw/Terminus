#pragma once

#include "stream.hpp"

struct File
{
    
};

class FileStream : public Stream
{
public:
    FileStream(File& file);
    ~FileStream();
    void seek(const size_t& offset) override;
    void write(void* src, const size_t& size) override;
    void read(void* dst, const size_t& size) override;
    void clear() override;
    void reserve(size_t capacity) override;
    
private:
    File& m_file;
};
