#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include <core/config.h>
#include <platform/platform.h>
#include <graphics/imgui_backend.h>
#include <graphics/renderer.h>
#include <graphics/imgui_helpers.h>
#include <graphics/imgui_console.h>
#include <memory/pool_allocator.h>
#include <memory/stack_allocator.h>
#include <core/global.h>
#include <io/filesystem.h>
#include <io/file_watcher.h>
#include <io/logger.h>
#include <input/input_handler.h>
#include <input/input_map.h>
#include <core/Event/event_handler.h>
#include <resource/shader_cache.h>
#include <resource/mesh_cache.h>
#include <resource/assimp_mesh_loader.h>
#include <resource/tsm_loader.h>
#include <resource/material_cache.h>
#include <resource/text_loader.h>
#include <resource/stb_image_loader.h>
#include <resource/texture_cache.h>
#include <resource/asset_common.h>
#include <resource/scene_cache.h>
#include <graphics/render_device.h>
#include <gameplay/scene_manager.h>
#include <gameplay/scene.h>
#include <gameplay/transform_system.h>
#include <math/math_utility.h>
#include <utility/mesh_importer.h>
#include <core/context.h>
#include <utility/profiler.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#define TERMINUS_DECLARE_MAIN(AppClass, PlatformClass)              \
int WINAPI WinMain(HINSTANCE hinstance,								\
									 HINSTANCE hprevinstance,		\
								     LPSTR lpcmdline,				\
									 int ncmdshow)					\
{																	\
	terminus::global::initialize();                                 \
    terminus::global::set_platform(T_NEW PlatformClass())           \
    terminus::Application* app = T_NEW AppClass();					\
	if(app->initialize())											\
	{																\
		app->run();													\
	}																\
	app->shutdown();												\
	terminus::global::shutdown();									\
	return 0;														\
}																	\

#else

#define TERMINUS_DECLARE_MAIN(AppClass, PlatformClass))				\
int main()															\
{																	\
	terminus::global::initialize();                                 \
    terminus::global::set_platform(T_NEW PlatformClass())           \
	terminus::Application* app = T_NEW AppClass();					\
	if(app->initialize())											\
	{																\
		app->run();													\
	}																\
	app->shutdown();												\
	terminus::global::shutdown();									\
	return 0;														\
}																	\

#endif


namespace terminus
{
	class Application
	{
	protected:
		DefaultThreadPool*   _main_thread_pool;
        
	public:
        Application() {}
        virtual ~Application() {}
        virtual bool initialize() = 0;
        virtual void run() = 0;
        virtual void shutdown() = 0;
	};

}

#endif
