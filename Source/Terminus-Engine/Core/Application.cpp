#include "Application.h"

#if defined(TERMINUS_PROFILING)
#define RMT_ENABLED 1
#include "../Utility/Remotery.h"
#endif

namespace Terminus {
    
#if defined(TERMINUS_PROFILING)
    Remotery* rmt;
#endif

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
        
#if defined(TERMINUS_PROFILING)
    rmt_CreateGlobalInstance(&rmt);
#endif

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
#if defined(TERMINUS_PROFILING)
            rmt_BeginCPUSample(GameLoop, 0);
#endif
            SubmitRendering();
			PlatformBackend::Update();
            EventHandler::Update();
            
            // Synchronize Rendering Thread
            m_rendering_thread_pool->Wait();
            
			Global::GetPerFrameAllocator()->Clear();
            
#if defined(TERMINUS_PROFILING)
            rmt_EndCPUSample();
#endif
		}
	}

	void Application::Shutdown()
	{
        ShutdownGraphics();
		PlatformBackend::Shutdown();
        
#if defined(TERMINUS_PROFILING)
        rmt_DestroyGlobalInstance(rmt);
#endif
	}
    
    void Application::SubmitRendering()
    {
#if defined(TERMINUS_PROFILING)
        rmt_BeginCPUSample(SubmitRendering, 0);
#endif
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::RenderingTask>(this);
        m_rendering_thread_pool->Submit();
#if defined(TERMINUS_PROFILING)
        rmt_EndCPUSample();
#endif
    }
    
    void Application::ShutdownGraphics()
    {
        TaskData* task = m_rendering_thread_pool->CreateTask();
        task->function.Bind<Application, &Application::GraphicsShutdownTask>(this);
        m_rendering_thread_pool->SubmitAndWait();
    }
    
    TASK_METHOD_DEFINITION(Application, RenderingTask)
    {
#if defined(TERMINUS_PROFILING)
        rmt_BeginCPUSample(RenderingTask, 0);
#endif
        m_render_device.BindFramebuffer(nullptr);
        
        m_render_device.ClearFramebuffer(FramebufferClearTarget::ALL, Vector4(0.3f, 0.3f,0.3f,1.0f));
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::new_frame();
        static bool testWin = true;
        ImGui::ShowTestWindow(&testWin);
        ImGuiBackend::render();
#endif
        
        m_render_device.SwapBuffers();
#if defined(TERMINUS_PROFILING)
        rmt_EndCPUSample();
#endif
    }
    
    TASK_METHOD_DEFINITION(Application, GraphicsInitializeTask)
    {
        m_render_device.Initialize();
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize(m_render_device);
#endif
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
