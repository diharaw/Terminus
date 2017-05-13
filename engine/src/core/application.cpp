#include <core/terminus.h>

#include <core/application.h>
#include <core/sync.h>
#include <platform/platform_sdl2.h>
#include <graphics/imgui_backend_sdl2.h>
#include <core/frame_packet.h>

namespace terminus
{
    TERMINUS_PROFILER_INSTANCE
    
    Application::Application()
    {
		_sim_pkt = &_pool[0];
		_cmd_pkt = &_pool[1];
		_dispatch_pkt = &_pool[2];
    }
    
    Application::~Application()
    {
        
    }
    
    bool Application::initialize()
    {
        return (initialize_engine() && initialize_app());
    }
    
    void Application::shutdown()
    {
        shutdown_app();
        shutdown_engine();
    }
    
    bool Application::initialize_engine()
    {
        _main_thread_pool = global::get_default_threadpool();
        
        TERMINUS_CREATE_PROFILER
        
        if (!context::get_platform()->initialize())
            return false;
        
        initialize_input();
        initialize_resources();
        initialize_ecs();
        initialize_physics();
        initialize_audio();
        initialize_script();
        
        Context& context = global::get_context();
        
        context._rendering_thread.run();
        context._loading_thread.run();
        
        
		context._renderer.initialize(&_pool[0]);
		sync::notify_loader_wakeup();
        
        return true;
    }

    void Application::shutdown_engine()
    {
        Context& context = global::get_context();
        
		context._physics_engine.shutdown();
        
        sync::notify_loader_wakeup();
        sync::wait_for_loader_exit();
        sync::wait_for_renderer_exit();
        
        context._loading_thread.exit();
        context._rendering_thread.exit();
        
        context::get_dynamic_library_cache().shutdown();
        context::get_platform()->shutdown();
        
        TERMINUS_DESTROY_PROFILER
    }
    
    void Application::initialize_input()
    {
        input_handler::initialize();
    }
    
    void Application::initialize_resources()
    {
        context::get_scene_cache().initialize();
    }
    
    void Application::initialize_physics()
    {
		context::get_physics_engine().initialize();
    }
    
    void Application::initialize_audio()
    {
        
    }
    
    void Application::initialize_ecs()
    {
        context::get_scene_manager().initialize();
    }
    
    void Application::initialize_script()
    {
        context::get_script_engine().initialize();
    }

	FramePacket* Application::sim_packet()
	{
		return _sim_pkt;
	}

	FramePacket* Application::cmd_packet()
	{
		return _cmd_pkt;
	}

	FramePacket* Application::dispatch_packet()
	{
		return _dispatch_pkt;
	}

	void Application::pass_packets()
	{
		FramePacket* temp = _sim_pkt;
		_sim_pkt = _dispatch_pkt;
		_dispatch_pkt = _cmd_pkt;
		_cmd_pkt = temp;
	}
}
