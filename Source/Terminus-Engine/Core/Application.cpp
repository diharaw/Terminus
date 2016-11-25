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
		if (!PlatformBackend::Initialize())
			return false;

		Input::Initialize();
		m_render_device.Initialize(nullptr, 0);

		m_world.Initialize();
		m_world.RegisterSystem(&m_transform_system);
		m_world.Initialize();

		return true;
	}

	void Application::Run()
	{
		while (!PlatformBackend::IsShutdownRequested())
		{
			PlatformBackend::Update();
			EventHandler::Update();
		}
	}

	void Application::Shutdown()
	{
		m_render_device.Shutdown();
		PlatformBackend::Shutdown();
	}

}