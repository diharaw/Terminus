#include <Core/application.h>

#if defined(TERMINUS_PROFILING)
#include <Utility/Remotery.h>
#endif

#include <Utility/dynamic_library.h>
#include <script/cpp_script.h>
#include <Utility/runtime_compile.h>

namespace terminus 
{
    // TEST TEST TEST
    
    int cmd_buf_idx;
    
    String scene_path = "";
    CppScript* test_script = nullptr;
    dynamic_library::LibHandle lib_handle;

    // TEST TEST TEST
    
    TERMINUS_PROFILER_INSTANCE
    
	Application::Application()
	{
        EventCallback callback;
        callback.Bind<Application, &Application::OnScenePreload>(this);
        EventHandler::register_listener(ScenePreloadEvent::sk_Type, callback);
        
        lib_handle = dynamic_library::open("libtestscript");
        if(dynamic_library::valid_handle(lib_handle))
        {
            T_LOG_INFO("Successfully loaded library");
            test_script = dynamic_library::create_instance_from_factory<CppScript>("CreateTestScript", lib_handle);
        }
        else
            T_LOG_ERROR("Failed to load library");
	}
    
    EVENT_METHOD_DEFINITION(Application, OnScenePreload)
    {
        ScenePreloadEvent* event_data = (ScenePreloadEvent*)event;
        T_LOG_INFO("Scene Load Complete");
    }

	Application::~Application()
	{
        if(dynamic_library::valid_handle(lib_handle))
            dynamic_library::close(lib_handle);
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
        
        // temp
        if(test_script)
            test_script->initialize();
        
		return true;
	}

	void Application::run()
	{
        Context& context = Global::get_context();
        Platform& platform = context::get_platform();
        Renderer& renderer = context::get_renderer();
        
        // TEST TEST TEST
        
        cmd_buf_idx = context::get_renderer().create_command_buffer();
        
        // TEST TEST TEST
        
        context._rendering_thread.run();
        context._loading_thread.run();
        
        context._main_ready_sema.notify();
        context._load_wakeup_sema.notify();
        context._render_ready_sema.wait();
        
        // begin initial scene load here 
        
		while (!context._shutdown)
		{
            TERMINUS_BEGIN_CPU_PROFILE(update_loop)
            
			platform.update();
            EventHandler::update();
            
            // temp
            if(test_script)
                test_script->update(0.0);
            
            context._scene_manager.update(0.0);
            // Synchronize Rendering Thread
            context._render_done_sema.wait();
            
            // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
            renderer.swap();
            
            temp_render();
            
			Global::GetPerFrameAllocator()->Clear();
            
            context._swap_done_sema.notify();
            
            TERMINUS_END_CPU_PROFILE
		}
	}
    
    void Application::temp_render()
    {
        // temp
        // TEST TEST TEST
        
        CommandBuffer& cmd_buf = context::get_renderer().command_buffer(cmd_buf_idx);
        
//        BindFramebufferCmdData cmd1;
//        cmd1.framebuffer = nullptr;
//        
//        cmd_buf.Write(CommandType::BindFramebuffer);
//        cmd_buf.Write(&cmd1, sizeof(cmd1));
//        
//        ClearFramebufferCmdData cmd2;
//        cmd2.clear_target = FramebufferClearTarget::ALL;
//        cmd2.clear_color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
//        
//        cmd_buf.Write(CommandType::ClearFramebuffer);
//        cmd_buf.Write(&cmd2, sizeof(cmd2));
//        
//        cmd_buf.WriteEnd();
        
        // TEST TEST TEST
        
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
        
        ImGui::Text("%s", scene_path.c_str());
        
        ImGui::SameLine();
        
        if(ImGui::Button("Browse..."))
        {
            scene_path = platform::open_file_dialog("json");
        }
        if(ImGui::Button("Load Scene"))
        {
            if(scene_path != "")
            {
                String trimmed_path = filesystem::get_file_name_and_extention(scene_path);
                std::cout << "Trimmed Path : " << trimmed_path << std::endl;
                context::get_scene_manager().load(trimmed_path);
            }
            else
            {
                std::cout << "Invalid Path" << std::endl;
            }
        }
        
        if(ImGui::Button("Reload Cpp Script"))
        {
            if(runtime_compile::compile("../../../Source/Terminus-Engine/script", "test_script.cpp", "../../../projects/bin/Debug/libtestscript", "-I \"../../Third Party/glm/\" -I \"../\" -I \"../../Third Party/sol2/\" -I \"../../Third Party/luajit/src/\" -I \"../../Third Party/rapidjson/\" -I \"../../Third Party/glew/include/\" -I \"../../Third Party/SDL2/include/\" -I \"../../Third Party/nfd/include/\" -I \"../../Third Party/Boxer/include/\"  -I \"../../Third Party/Delegates11/\"", "-std=c++14"))
            {
                T_LOG_INFO("Successfully compiled");
                
                dynamic_library::close(lib_handle);
                
                lib_handle = dynamic_library::open("libtestscript");
                if(dynamic_library::valid_handle(lib_handle))
                {
                    T_LOG_INFO("Successfully loaded library");
                    
                    if(test_script)
                    {
                        test_script->shutdown();
                        delete test_script;
                    }
                    
                    test_script = dynamic_library::create_instance_from_factory<CppScript>("CreateTestScript", lib_handle);
                }
                else
                    T_LOG_ERROR("Failed to load library");
            }
            else
                T_LOG_ERROR("Failed to compile");
        }
        
        ImGui::End();
        
        // TEST
#endif
    }

	void Application::shutdown()
	{
        Context& context = Global::get_context();
        
        context._load_wakeup_sema.notify();
        context._load_exit_sema.wait();
        context._render_exit_sema.wait();
        
        context._loading_thread.exit();
        context._rendering_thread.exit();
        
        context::get_platform().shutdown();
        
        // temp
        if(test_script)
            test_script->shutdown();
        
        TERMINUS_DESTROY_PROFILER
	}

	void Application::initialize_input()
	{
		Input::Initialize();
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

	}
}
