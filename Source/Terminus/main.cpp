#include "Platform/PlatformBackend.h"
#include "Graphics/RenderBackend.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/RenderConfigUI.h"
#include "Graphics/CommandList.h"
#include "Graphics/RenderSystem.h"
#include <glm.hpp>
#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "GlobalMemory.h"
#include "IO/FileSystem.h"
#include "IO/FileWatcher.h"
#include "Input/Input.h"
#include "Input/InputContext.h"
#include "Core/Event/EventHandler.h"
#include "Resource/StbLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include <iostream>

class Test
{
public:
    Test()
    {
        EventCallback callback;
        callback.Bind<Test, &Test::OnStateInput>(this);
        Terminus::EventHandler::RegisterListener(InputStateEvent::sk_Type,  callback);
        
        callback.Bind<Test, &Test::OnActionInput>(this);
        Terminus::EventHandler::RegisterListener(InputActionEvent::sk_Type, callback);
        
        callback.Bind<Test, &Test::OnAxisInput>(this);
        Terminus::EventHandler::RegisterListener(InputAxisEvent::sk_Type, callback);
    }
    
    void OnStateInput(Event* _event)
    {
        InputStateEvent* event = (InputStateEvent*)_event;
        
        std::cout << event->GetName() << " "  <<  event->GetState() << std::endl;
    }
    
    void OnActionInput(Event* _event)
    {
        InputActionEvent* event = (InputActionEvent*)_event;
        
        std::string action;
        
        if(event->GetValue() == 0)
            action = "Released";
        else
            action = "Pressed";
        
        std::cout << event->GetName() << " "  << action << std::endl;

    }
    
    void OnAxisInput(Event* _event)
    {
        InputAxisEvent* event = (InputAxisEvent*)_event;
        std::cout << event->GetName() << " Axis : "  << event->GetAxis() << ", Value : " << event->GetValue() << " , Delta : " << event->GetDelta() << std::endl;
    }
};

int main(void)
{
    Terminus::Memory::Initialize();

    FileSystem::add_directory("Assets");
	
    FileWatcher::add_directory("Assets");
    
    if(!PlatformBackend::Initialize(800, 600))
        return -1;
    
    Input::Initialize();
    
    RenderBackend::Initialize();
	imgui_backend::initialize();
    
    InputContext* context = Input::CreateContext();
    context->m_ContextName = "Test";
    context->m_KeyboardStateMap[GLFW_KEY_E] = "HELLO WORLD";
    
    context->m_KeyboardActionMap[GLFW_KEY_Q] = "Fire";
    
    context->m_KeyboardAxisPositiveMap[GLFW_KEY_W] = "Forward";
    context->m_KeyboardAxisNegativeMap[GLFW_KEY_S] = "Forward";

    Input::SetActiveContext("Test");
    
    Test test;
    
	TextureCache cache;

	cache.RegisterLoader<StbLoader>();

	ResourceHandle handle = cache.Load("Test.png");
	handle = cache.Load("Test.png");

    while(!PlatformBackend::IsShutdownRequested())
    {
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		imgui_backend::new_frame();
        PlatformBackend::Update();
        FileWatcher::update();
        Terminus::EventHandler::Update();
        
		bool open = true;

		ImGui::ShowTestWindow(&open);
		render_config_ui::render_pipeline();
		imgui_backend::render();
		RenderBackend::SwapBuffers();
    }
    
	imgui_backend::shutdown();
    PlatformBackend::Shutdown();
    
    Terminus::Memory::Shutdown();
    
    return 0;
}
