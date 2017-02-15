#include <Core/application.h>

#if defined(TERMINUS_PROFILING)
#include <Utility/Remotery.h>
#endif

#include <Utility/runtime_compile.h>

namespace terminus 
{
    // TEST TEST TEST
    
    String scene_path = "";
    // TEST TEST TEST
    
    TERMINUS_PROFILER_INSTANCE
    
	Application::Application()
	{
        EventCallback callback;
        callback.Bind<Application, &Application::OnScenePreload>(this);
        EventHandler::register_listener(ScenePreloadEvent::sk_Type, callback);
	}
    
    EVENT_METHOD_DEFINITION(Application, OnScenePreload)
    {
        ScenePreloadEvent* event_data = (ScenePreloadEvent*)event;
        T_LOG_INFO("Scene Load Complete");
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
        
#if defined(TERMINUS_WITH_EDITOR)
        Editor& editor = context::get_editor();
        editor.initialize();
#endif
        
        context._rendering_thread.run();
        context._loading_thread.run();
        
        context._main_ready_sema.notify();
        context._load_wakeup_sema.notify();
        context._render_ready_sema.wait();
        
        // begin initial scene load here 
#if !defined(TERMINUS_WITH_EDITOR)
		load_initial_scene();
#endif
        
		while (!context._shutdown)
		{
            TERMINUS_BEGIN_CPU_PROFILE(update_loop)
            
            platform.begin_frame();
			platform.update();
            EventHandler::update();
            
            context._scene_manager.update(platform.get_delta_time());
            // Synchronize Rendering Thread
            context._render_done_sema.wait();
            
            // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
            renderer.swap();
            
            //temp_render();
            editor.update(platform.get_delta_time());
            
			Global::GetPerFrameAllocator()->Clear();
            
            context._swap_done_sema.notify();
            platform.end_frame();
            
            TERMINUS_END_CPU_PROFILE
		}
	}

	void Application::load_initial_scene()
	{
		SceneManager& scene_manager = context::get_scene_manager();
		
		if (context::get_engine_config()->has_value(CONFIG_INITIAL_SCENE))
		{
			StringBuffer64 scene_name = context::get_engine_config()->get_string(CONFIG_INITIAL_SCENE);
			scene_manager.load(String(scene_name.c_str()));
		}
	}
    
    void Application::temp_render()
    {
#if defined(TERMINUS_WITH_EDITOR)
        static bool testWin = true;
        ImGui::ShowTestWindow(&testWin);
        
        // TEST
        ImGui::SetNextWindowSize(ImVec2(900,400), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Console", &testWin, 0);
        imgui_console::draw();
        ImGui::End();
        
        ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Scene Load", &testWin, 0);
        
        if(ImGui::Button("Reload Lua Script"))
        {
            LuaScriptUpdatedEvent temp("test_lua_script.lua");
            context::get_script_engine().on_lua_script_updated(&temp);
        }

        ImGui::End();
        
        // TEST
#endif
    }

	void Application::shutdown()
	{
        Context& context = Global::get_context();
        
        context._editor.shutdown();
        
        context._load_wakeup_sema.notify();
        context._load_exit_sema.wait();
        context._render_exit_sema.wait();
        
        context._loading_thread.exit();
        context._rendering_thread.exit();
        
        context::get_dynamic_library_cache().shutdown();
        context::get_platform().shutdown();
        
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
}
