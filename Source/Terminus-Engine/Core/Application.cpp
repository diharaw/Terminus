#include "Application.h"

namespace Terminus {

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

		if (!PlatformBackend::Initialize())
			return false;

		InitializeInput();
		InitializeGraphics();
		InitializeResources();
		InitializeECS();
		InitializePhysics();
		InitializeAudio();
		InitializeScript();
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize(m_render_device);
#endif
        
		return true;
	}

	void Application::Run()
	{
		m_render_device.BindFramebuffer(nullptr);

		while (!PlatformBackend::IsShutdownRequested())
		{
			PlatformBackend::Update();
            EventHandler::Update();
            
#if defined(TERMINUS_WITH_EDITOR)
            ImGuiBackend::new_frame();
            static bool testWin = true;
            ImGui::ShowTestWindow(&testWin);
            ImGuiBackend::render();
#endif
            
            m_render_device.SwapBuffers();
			Global::GetPerFrameAllocator()->Clear();
		}
	}

	void Application::Shutdown()
	{
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::shutdown();
#endif
		m_render_device.Shutdown();
		PlatformBackend::Shutdown();
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
	}

	void Application::InitializeGraphics()
	{
		m_render_device.Initialize();
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
