#include "Application.h"

#if defined(TERMINUS_PROFILING)
#include "../Utility/Remotery.h"
#endif

namespace Terminus {
    
    TERMINUS_PROFILER_INSTANCE

    // TEST TEST TEST
    
    int cmd_buf_idx;
    
    // TEST TEST TEST
    
	Application::Application()
	{
        
	}

	Application::~Application()
	{
        
	}
    
	bool Application::initialize()
	{
		m_main_thread_pool = Global::GetDefaultThreadPool();
        m_rendering_thread_pool = Global::GetRenderingThreadPool();
        
        TERMINUS_CREATE_PROFILER

		if (!platform::initialize())
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
		while (!platform::shutdown_requested())
		{
            TERMINUS_BEGIN_CPU_PROFILE(GameLoop)
            
            submit_rendering();
			platform::update();
            EventHandler::Update();
            
            // Synchronize Rendering Thread
            m_rendering_thread_pool->Wait();
            
            // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
            m_renderer.swap();
            
			Global::GetPerFrameAllocator()->Clear();
            
            TERMINUS_END_CPU_PROFILE
		}
	}

	void Application::shutdown()
	{
        shutdown_graphics();
		platform::shutdown();
        
        TERMINUS_DESTROY_PROFILER
	}
    
    void Application::submit_rendering()
    {
        TERMINUS_BEGIN_CPU_PROFILE(submit_rendering);

        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::rendering_task>(this);
        m_rendering_thread_pool->Submit();

        TERMINUS_END_CPU_PROFILE
    }
    
    void Application::shutdown_graphics()
    {
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::graphics_shutdown_task>(this);
        m_rendering_thread_pool->SubmitAndWait();
    }
    
    TASK_METHOD_DEFINITION(Application, rendering_task)
    {
        TERMINUS_BEGIN_CPU_PROFILE(rendering_task)

        // TEST TEST TEST
        
        Graphics::CommandBuffer& cmd_buf = m_renderer.command_buffer(cmd_buf_idx);
        
        Graphics::BindFramebufferCmdData cmd1;
        cmd1.framebuffer = nullptr;
    
        cmd_buf.Write(Graphics::CommandType::BindFramebuffer);
        cmd_buf.Write(&cmd1, sizeof(cmd1));
        
        Graphics::ClearFramebufferCmdData cmd2;
        cmd2.clear_target = FramebufferClearTarget::COLOR;
        cmd2.clear_color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
        
        cmd_buf.Write(Graphics::CommandType::ClearFramebuffer);
        cmd_buf.Write(&cmd2, sizeof(cmd2));
        
        cmd_buf.WriteEnd();
        
        // TEST TEST TEST
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::new_frame();
        static bool testWin = true;
        ImGui::ShowTestWindow(&testWin);
#endif
        m_renderer.submit(m_render_device);
        
        TERMINUS_END_CPU_PROFILE
    }
    
    TASK_METHOD_DEFINITION(Application, graphics_initialize_task)
    {
        m_render_device.Initialize();
        m_renderer.initialize(m_render_device);
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize(m_render_device);
#endif
        
        // TEST TEST TEST
        
        cmd_buf_idx = m_renderer.create_command_buffer();
        
        // TEST TEST TEST
    }
    
    TASK_METHOD_DEFINITION(Application, graphics_shutdown_task)
    {
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::shutdown();
#endif
        m_render_device.Shutdown();
    }

	void Application::initialize_input()
	{
		Input::Initialize();
	}

	void Application::initialize_resources()
	{
		m_texture_cache.Initialize(&m_render_device);
		m_material_cache.Initialize(&m_render_device, &m_texture_cache);
		m_mesh_cache.Initialize(&m_render_device, &m_material_cache);
		m_shader_cache.Initialize(&m_render_device);
        m_scene_cache.Initialize(&m_mesh_cache);
	}

	void Application::initialize_graphics()
	{
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::graphics_initialize_task>(this);
        m_rendering_thread_pool->SubmitAndWait();
	}

	void Application::initialize_physics()
	{

	}

	void Application::initialize_audio()
	{
        
	}

	void Application::initialize_ecs()
	{
		m_scene_manager.Initialize(&m_scene_cache, &m_render_device);
	}

	void Application::initialize_script()
	{

	}

}
