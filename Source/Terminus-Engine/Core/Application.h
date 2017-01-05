#pragma once

#ifndef  APPLICATION_H
#define APPLICATION_H

#include "Core/Config.h"
#include "Platform/platform.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/renderer.h"
#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "Global.h"
#include "IO/FileSystem.h"
#include "IO/FileWatcher.h"
#include "Input/Input.h"
#include "Input/InputContext.h"
#include "Core/Event/EventHandler.h"
#include "Resource/ShaderCache.h"
#include "Resource/MeshCache.h"
#include "Resource/AssimpMeshLoader.h"
#include "Resource/TSMLoader.h"
#include "Resource/MaterialCache.h"
#include "Resource/TextLoader.h"
#include "Resource/StbImageLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include "Resource/SceneCache.h"
#include "Graphics/render_device.h"
#include "ECS/SceneManager.h"
#include "Math/MathUtility.h"
#include "Utility/MeshImporter.h"
#include "ECS/Scene.h"
#include "ECS/TransformSystem.h"
#include "context.h"

#if defined(TERMINUS_PLATFORM_WIN32)
#define TERMINUS_DECLARE_MAIN(x)							\
int WINAPI WinMain(HINSTANCE hinstance,			    \
									 HINSTANCE hprevinstance,			\
								     LPSTR lpcmdline,							\
									 int ncmdshow)								\
{																							\
	terminus::Global::Initialize();											\
	terminus::Application* app = T_NEW x();					\
	if(app->initialize())														\
	{																						\
		app->run();																\
	}																						\
	app->shutdown();															\
	terminus::Global::Shutdown();										\
	return 0;																			\
}																							\

#else

#define TERMINUS_DECLARE_MAIN(x)			\
int main()															\
{																			\
	terminus::Global::Initialize();							\
	terminus::Application* app = T_NEW x();	\
	if(app->initialize())										\
	{																		\
		app->run();												\
	}																		\
	app->shutdown();											\
	terminus::Global::Shutdown();						\
	return 0;															\
}																			\

#endif


namespace terminus {

	class Application
	{
	private:
		ThreadPool*					   m_main_thread_pool;
        ThreadPool*                    m_rendering_thread_pool;
        
	public:
		Application();
		~Application();
		bool initialize();
		void run();
		void shutdown();

	private:
		void initialize_input();
		void initialize_resources();
		void initialize_graphics();
		void initialize_physics();
		void initialize_audio();
		void initialize_ecs();
		void initialize_script();
        
        void submit_rendering();
        void shutdown_graphics();
        
        TASK_METHOD_DECLARATION(graphics_initialize_task);
        TASK_METHOD_DECLARATION(graphics_shutdown_task);
        TASK_METHOD_DECLARATION(rendering_task);
        
        EVENT_METHOD_DECLARATION(OnScenePreload);
	};

}

#endif
