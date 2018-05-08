#include <core/engine_core.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

namespace global
{
	EngineCore* g_engine_core;

	void initialize_engine_core(Application* app)
	{
		g_engine_core = new EngineCore();
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
		delete g_engine_core;
	}
}

TE_END_TERMINUS_NAMESPACE