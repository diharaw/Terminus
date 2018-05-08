#pragma once

#include <core/terminus_macros.hpp>
#include <core/application.hpp>
#include <memory/heap_allocator.hpp>
#include <io/filesystem.hpp>
#include <io/input_manager.hpp>
#include <event/event_manager.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct EngineCore
{
	HeapAllocator m_default_allocator;
	FileSystem	  m_filesystem;
	EventManager  m_event_manager;
	InputManager  m_input_manager;

	// Application 
	Application*  m_application;
};

namespace global
{
	extern void		   initialize_engine_core(Application* app);
	extern EngineCore* engine_core();
	extern void		   reset_engine_core(EngineCore* core);
	extern void		   shutdown_engine_core();
	
	inline HeapAllocator& default_allocator() { return engine_core()->m_default_allocator; }
	inline FileSystem&	  filesystem()		  { return engine_core()->m_filesystem; }
	inline EventManager&  event_manager()	  { return engine_core()->m_event_manager; }
	inline InputManager&  input_manager()	  { return engine_core()->m_input_manager; }
	inline Application*   application()		  { return engine_core()->m_application; }
}

TE_END_TERMINUS_NAMESPACE