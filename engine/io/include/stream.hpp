#pragma once

#include <stdio.h>

class Stream
{
public:
    Stream() {}
    virtual ~Stream() {}
    virtual void seek(const size_t& offset) = 0;
    virtual void write(void* src, const size_t& size) = 0;
    virtual void read(void* dst, const size_t& size) = 0;
    virtual void clear() = 0;
    virtual void reserve(size_t capacity) = 0;
};
