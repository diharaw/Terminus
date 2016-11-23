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
		Global::Initialize();

		if (!PlatformBackend::Initialize(1280, 720))
			return false;

		Input::Initialize();
		m_render_device.Initialize(nullptr, 0);
		
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
		Global::Shutdown();
	}

}