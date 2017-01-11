#include <Core/application.h>

#if defined(TERMINUS_PROFILING)
#include <Utility/Remotery.h>
#endif

namespace terminus 
{    
    TERMINUS_PROFILER_INSTANCE
    
    String scene_path = "";
    bool file_dialog = false;
    
    // TEST TEST TEST
    
    int cmd_buf_idx;
    
    // TEST TEST TEST
    
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
        _rendering_thread_pool = Global::GetRenderingThreadPool();
        
        TERMINUS_CREATE_PROFILER

		if (!context::get_platform().initialize())
			return false;

		initialize_input();
		initialize_graphics();
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
            
            if(file_dialog)
            {
                scene_path = platform::open_file_dialog("json");
                file_dialog = false;
            }
            
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
        shutdown_graphics();
        context::get_platform().shutdown();
        
        TERMINUS_DESTROY_PROFILER
	}
    
    void Application::submit_rendering()
    {
        TERMINUS_BEGIN_CPU_PROFILE(submit_rendering);

        Task task;
        task._function.Bind<Application, &Application::rendering_task>(this);
        _rendering_thread_pool->enqueue(task);

        TERMINUS_END_CPU_PROFILE
    }
    
    void Application::shutdown_graphics()
    {
        Task task;
        task._function.Bind<Application, &Application::graphics_shutdown_task>(this);
        _rendering_thread_pool->enqueue(task);
        _rendering_thread_pool->wait();
    }
    
    TASK_METHOD_DEFINITION(Application, rendering_task)
    {
        TERMINUS_BEGIN_CPU_PROFILE(rendering_task)

        // TEST TEST TEST
        
        CommandBuffer& cmd_buf = context::get_renderer().command_buffer(cmd_buf_idx);
        
        BindFramebufferCmdData cmd1;
        cmd1.framebuffer = nullptr;
    
        cmd_buf.Write(CommandType::BindFramebuffer);
        cmd_buf.Write(&cmd1, sizeof(cmd1));
        
        ClearFramebufferCmdData cmd2;
        cmd2.clear_target = FramebufferClearTarget::ALL;
        cmd2.clear_color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        
        cmd_buf.Write(CommandType::ClearFramebuffer);
        cmd_buf.Write(&cmd2, sizeof(cmd2));
        
        cmd_buf.WriteEnd();
        
        // TEST TEST TEST
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::new_frame();
        static bool testWin = true;
        ImGui::ShowTestWindow(&testWin);
        
        // TEST

        ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Scene Load", &testWin, 0);
        
        ImGui::Text("%s", scene_path.c_str());
        
        ImGui::SameLine();
        
        if(ImGui::Button("Browse..."))
        {
            file_dialog = true;
        }
        if(ImGui::Button("Load Scene"))
        {
            if(scene_path != "")
            {
                String trimmed_path = filesystem::get_file_name_and_extention(scene_path);
                std::cout << "Trimmed Path : " << trimmed_path << std::endl;
                context::get_scene_manager().Preload(trimmed_path);
            }
            else
            {
                std::cout << "Invalid Path" << std::endl;
            }
        }
        
        ImGui::End();
        // TEST
        
#endif
        context::get_renderer().submit();
        
        TERMINUS_END_CPU_PROFILE
    }
    
    TASK_METHOD_DEFINITION(Application, graphics_initialize_task)
    {
        context::get_render_device().Initialize();
        context::get_renderer().initialize();
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize();
#endif
        
        // TEST TEST TEST
        
        cmd_buf_idx = context::get_renderer().create_command_buffer();
        
        // TEST TEST TEST
    }
    
    TASK_METHOD_DEFINITION(Application, graphics_shutdown_task)
    {
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::shutdown();
#endif
        context::get_render_device().Shutdown();
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

	void Application::initialize_graphics()
	{
        Task task;
        task._function.Bind<Application, &Application::graphics_initialize_task>(this);
        _rendering_thread_pool->enqueue(task);
        _rendering_thread_pool->wait();
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
