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
		m_thread_pool = Global::GetDefaultThreadPool();

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
			PlatformBackend::Update();
			EventHandler::Update();
            m_render_device.ClearFramebuffer(FramebufferClearTarget::ALL, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            m_render_device.SwapBuffers();
			Global::GetPerFrameAllocator()->Clear();
		}
	}

	void Application::Shutdown()
	{
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
		m_render_device.Initialize(nullptr, 0);
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
