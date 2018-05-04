#pragma once

#include <core/include/terminus_macros.hpp>
#include <memory/src/heap_allocator.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct EngineCore
{
	HeapAllocator m_default_allocator;
};

namespace global
{
	extern void		   initialize_engine_core();
	extern EngineCore* engine_core();
	extern void		   reset_engine_core(EngineCore* core);
	extern void		   shutdown_engine_core();

	inline HeapAllocator& default_allocator() { return engine_core()->m_default_allocator; }
}

TE_END_TERMINUS_NAMESPACE