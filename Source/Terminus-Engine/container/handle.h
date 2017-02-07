#pragma once

#include <stdint.h>

#define INVALID_HINDEX USHRT_MAX
#define INVALID_HGEN USHRT_MAX
#define VALID_HANDLE(x) x._index != INVALID_HINDEX && x._generation != INVALID_HGEN

template<typename T, uint32_t INDEX_SIZE = 16, uint32_t GEN_SIZE = 16>
struct Handle
{
	uint32_t _index		 : INDEX_SIZE;
	uint32_t _generation : GEN_SIZE;
    
    Handle()
    {
        _index = INVALID_HINDEX;
        _generation = INVALID_HGEN;
    }
};
