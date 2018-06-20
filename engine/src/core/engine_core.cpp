#include <core/engine_core.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

namespace global
{
	EngineCore* g_engine_core;
	HeapAllocator g_heap_allocator;

	HeapAllocator& default_allocator()
	{
		return g_heap_allocator;
	}

	void initialize_engine_core(Application* app)
	{
		g_engine_core = TE_HEAP_NEW EngineCore();
		g_engine_core->m_application = app;
	}

	EngineCore*	engine_core()
	{
		return g_engine_core;
	}

	void reset_engine_core(EngineCore* core)
	{
		if (core)
			g_engine_core = core;
	}

	void shutdown_engine_core()
	{
		TE_HEAP_DELETE(g_engine_core);
	}
}

TE_END_TERMINUS_NAMESPACE