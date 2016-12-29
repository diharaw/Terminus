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
    
	bool Application::Initialize()
	{
		m_main_thread_pool = Global::GetDefaultThreadPool();
        m_rendering_thread_pool = Global::GetRenderingThreadPool();
        
        TERMINUS_CREATE_PROFILER

		if (!PlatformBackend::Initialize())
			return false;

		InitializeInput();
		InitializeGraphics();
		InitializeResources();
		InitializeECS();
		InitializePhysics();
		InitializeAudio();
		InitializeScript();
        
		return true;
	}

	void Application::Run()
	{
		while (!PlatformBackend::IsShutdownRequested())
		{
            TERMINUS_BEGIN_CPU_PROFILE(GameLoop)
            
            SubmitRendering();
			PlatformBackend::Update();
            EventHandler::Update();
            
            // Synchronize Rendering Thread
            m_rendering_thread_pool->Wait();
            
			Global::GetPerFrameAllocator()->Clear();
            
            TERMINUS_END_CPU_PROFILE
		}
	}

	void Application::Shutdown()
	{
        ShutdownGraphics();
		PlatformBackend::Shutdown();
        
        TERMINUS_DESTROY_PROFILER
	}
    
    void Application::SubmitRendering()
    {
        TERMINUS_BEGIN_CPU_PROFILE(SubmitRendering);

        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::RenderingTask>(this);
        m_rendering_thread_pool->Submit();

        TERMINUS_END_CPU_PROFILE
    }
    
    void Application::ShutdownGraphics()
    {
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::GraphicsShutdownTask>(this);
        m_rendering_thread_pool->SubmitAndWait();
    }
    
    TASK_METHOD_DEFINITION(Application, RenderingTask)
    {
        TERMINUS_BEGIN_CPU_PROFILE(RenderingTask)

        // TEST TEST TEST
        
        Graphics::CommandBuffer& cmd_buf = m_renderer.GetGraphicsQueueFront().m_cmd_buf[cmd_buf_idx];
        
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
        m_renderer.Submit(m_render_device);
        
        TERMINUS_END_CPU_PROFILE
    }
    
    TASK_METHOD_DEFINITION(Application, GraphicsInitializeTask)
    {
        m_render_device.Initialize();
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize(m_render_device);
#endif
        
        // TEST TEST TEST
        
        cmd_buf_idx = m_renderer.GetGraphicsQueueFront().CreateCommandBuffer();
        m_renderer.GetGraphicsQueueBack().CreateCommandBuffer();
        
        // TEST TEST TEST
    }
    
    TASK_METHOD_DEFINITION(Application, GraphicsShutdownTask)
    {
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::shutdown();
#endif
        m_render_device.Shutdown();
    }

	void Application::InitializeInput()
	{
		Input::Initialize();
	}

	void Application::InitializeResources()
	{
		m_texture_cache.Initialize(&m_render_device);
		m_material_cache.Initialize(&m_render_device, &m_texture_cache);
		m_mesh_cache.Initialize(&m_render_device, &m_material_cache);
		m_shader_cache.Initialize(&m_render_device);
        m_scene_cache.Initialize(&m_mesh_cache);
	}

	void Application::InitializeGraphics()
	{
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::GraphicsInitializeTask>(this);
        m_rendering_thread_pool->SubmitAndWait();
	}

	void Application::InitializePhysics()
	{

	}

	void Application::InitializeAudio()
	{

	}

	void Application::InitializeECS()
	{
		m_scene_manager.Initialize(&m_scene_cache);
	}

	void Application::InitializeScript()
	{

	}

}
