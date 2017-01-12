#include <Core/application.h>

#if defined(TERMINUS_PROFILING)
#include <Utility/Remotery.h>
#endif

namespace terminus 
{    
    TERMINUS_PROFILER_INSTANCE
    
	Application::Application()
	{
        EventCallback callback;
        callback.Bind<Application, &Application::OnScenePreload>(this);
        EventHandler::RegisterListener(ScenePreloadEvent::sk_Type, callback);
	}
    
    EVENT_METHOD_DEFINITION(Application, OnScenePreload)
    {
        ScenePreloadEvent* event_data = (ScenePreloadEvent*)event;
        std::cout << "Scene Load Complete" << std::endl;
    }

	Application::~Application()
	{
        
	}
    
	bool Application::initialize()
	{
		_main_thread_pool = Global::GetDefaultThreadPool();
        
        TERMINUS_CREATE_PROFILER

		if (!context::get_platform().initialize())
			return false;

		initialize_input();
		initialize_resources();
		initialize_ecs();
		initialize_physics();
		initialize_audio();
		initialize_script();
        
		return true;
	}

	void Application::run()
	{
        Context& context = Global::get_context();
        Platform& platform = context::get_platform();
        Renderer& renderer = context::get_renderer();
        
        context._rendering_thread.run();
        context._loading_thread.run();
        
        context._main_ready_sema.notify();
        context._render_ready_sema.wait();
        
		while (!context._shutdown)
		{
            TERMINUS_BEGIN_CPU_PROFILE(GameLoop)
            
			platform.update();
            EventHandler::Update();
                       
            // Synchronize Rendering Thread
            context._render_done_sema.wait();
            
            // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
            renderer.swap();
            
			Global::GetPerFrameAllocator()->Clear();
            
            context._swap_done_sema.notify();
            
            TERMINUS_END_CPU_PROFILE
		}
	}

	void Application::shutdown()
	{
        context::get_platform().shutdown();
        
        TERMINUS_DESTROY_PROFILER
	}

	void Application::initialize_input()
	{
		Input::Initialize();
	}

	void Application::initialize_resources()
	{
        context::get_texture_cache().Initialize();
        context::get_material_cache().Initialize();
		context::get_mesh_cache().Initialize();
		context::get_shader_cache().Initialize();
        context::get_scene_cache().Initialize();
	}

	void Application::initialize_physics()
	{

	}

	void Application::initialize_audio()
	{
        
	}

	void Application::initialize_ecs()
	{
        context::get_scene_manager().Initialize();
	}

	void Application::initialize_script()
	{

	}
}
