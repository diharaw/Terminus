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
#include "Resource/StbLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include <iostream>

int main(void)
{
    Terminus::Memory::Initialize();

    FileSystem::AddDirectory("Assets");
	
    if(!PlatformBackend::Initialize(800, 600))
        return -1;
    
    RenderBackend::Initialize();
	imgui_backend::initialize();

	TextureCache cache;

	cache.RegisterLoader<StbLoader>();

	Texture2D* handle = cache.Load("Test.png");
	handle = cache.Load("Test.png");

    while(!PlatformBackend::IsShutdownRequested())
    {
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		imgui_backend::new_frame();
        PlatformBackend::Update();

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
