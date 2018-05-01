#pragma once

#include <core/include/terminus_macros.hpp>
#include <stdio.h>

TE_BEGIN_TERMINUS_NAMESPACE

class IStream
{
public:
	IStream() {}
    virtual ~IStream() {}
    virtual void seek(const size_t& offset) = 0;
    virtual void write(void* src, const size_t& size) = 0;
    virtual void read(void* dst, const size_t& size) = 0;
    virtual void reset() = 0;
	virtual size_t size() = 0;
	inline void set_auto_move_write(bool auto_move) { m_auto_move_write = auto_move; }
	inline void set_auto_move_read(bool auto_move)  { m_auto_move_read = auto_move;  }

protected:
	bool m_auto_move_write = false;
	bool m_auto_move_read = false;
	size_t m_pointer = 0;
};

TE_END_TERMINUS_NAMESPACE