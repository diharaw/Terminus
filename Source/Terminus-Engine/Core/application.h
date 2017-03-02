#pragma once

#ifndef  APPLICATION_H
#define APPLICATION_H

#include <Core/config.h>
#include <Platform/platform.h>
#include <Graphics/imgui_backend.h>
#include <Graphics/renderer.h>
#include <Graphics/imgui_helpers.h>
#include <Graphics/imgui_console.h>
#include <Memory/pool_allocator.h>
#include <Memory/stack_allocator.h>
#include <global.h>
#include <IO/filesystem.h>
#include <IO/file_watcher.h>
#include <IO/logger.h>
#include <Input/input_handler.h>
#include <Input/input_map.h>
#include <Core/Event/event_handler.h>
#include <Resource/shader_cache.h>
#include <Resource/mesh_cache.h>
#include <Resource/assimp_mesh_loader.h>
#include <Resource/tsm_loader.h>
#include <Resource/material_cache.h>
#include <Resource/text_loader.h>
#include <Resource/stb_image_loader.h>
#include <Resource/texture_cache.h>
#include <Resource/asset_common.h>
#include <Resource/scene_cache.h>
#include <Graphics/render_device.h>
#include <ECS/scene_manager.h>
#include <Math/math_utility.h>
#include <Utility/mesh_importer.h>
#include <ECS/scene.h>
#include <ECS/transform_system.h>
#include <Core/context.h>
#include <Utility/profiler.h>

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
		DefaultThreadPool*   _main_thread_pool;
        
	public:
		Application();
		~Application();
		bool initialize();
		void run();
		void shutdown();

	private:
		void initialize_input();
		void initialize_resources();
		void initialize_physics();
		void initialize_audio();
		void initialize_ecs();
		void initialize_script();
		void load_initial_scene();

        EVENT_METHOD_DECLARATION(OnScenePreload);
        
        void temp_render();
	};

}

#endif
