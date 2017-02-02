#pragma once

#include <stdint.h>

template<typename T, uint32_t INDEX_SIZE = 16, uint32_t GEN_SIZE = 16>
struct Handle
{
	uint32_t _index		 : INDEX_SIZE;
	uint32_t _generation : GEN_SIZE;
};
