#pragma once

#include <core/terminus_macros.hpp>
#include <core/application.hpp>
#include <memory/heap_allocator.hpp>
#include <io/filesystem.hpp>
#include <io/input_manager.hpp>
#include <event/event_manager.hpp>
#include <gfx/gfx_device.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct EngineCore
{

	FileSystem	  m_filesystem;
	EventManager  m_event_manager;
	InputManager  m_input_manager;
	GfxDevice	  m_gfx_device;

	// Application 
	Application*  m_application;
};

namespace global
{
	extern HeapAllocator& default_allocator();

	extern void		   initialize_engine_core(Application* app);
	extern EngineCore* engine_core();
	extern void		   reset_engine_core(EngineCore* core);
	extern void		   shutdown_engine_core();
	
	inline FileSystem&	  filesystem()		  { return engine_core()->m_filesystem; }
	inline EventManager&  event_manager()	  { return engine_core()->m_event_manager; }
	inline InputManager&  input_manager()	  { return engine_core()->m_input_manager; }
	inline GfxDevice&	  gfx_device()		  { return engine_core()->m_gfx_device; }
	inline Application*   application()		  { return engine_core()->m_application; }
}

TE_END_TERMINUS_NAMESPACE