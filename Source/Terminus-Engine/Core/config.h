#ifndef CONFIG_H
#define CONFIG_H

#define TERMINUS_BUILD 161108

#define TERMINUS_OPENGL

#define TERMINUS_ENABLE_ERROR_CHECK

#ifdef __APPLE__
    #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_METAL)
        #define TERMINUS_OPENGL
    #endif
#else
    #ifdef WIN32
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_DIRECT3D11)
            #define TERMINUS_DIRECT3D11
        #endif
    #else
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_VULKAN)
            #define TERMINUS_OPENGL
        #endif
    #endif
#endif

#endif

// Terminus Platform

#if defined(__APPLE__)
    #include "TargetConditionals.h"
    #if !TARGET_OS_IPHONE
        #define TERMINUS_PLATFORM_MACOS
    #elif TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #define TERMINUS_PLATFORM_IOS
    #endif
#elif defined(WIN32)
    #define TERMINUS_PLATFORM_WIN32
#else
    #define TERMINUS_PLATFORM_LINUX
#endif

// Compiler Type

#if defined(__clang__)
	#define TERMINUS_COMPILER_CLANG
#elif defined(__GNUC__)
	#define TERMINUS_COMPILER_GCC
#elif defined(_MSC_VER)
	#define TERMINUS_COMPILER_MSVC
#endif

// Build Type

#define TERMINUS_WITH_EDITOR

#define TERMINUS_PROFILING

#if defined(TERMINUS_PROFILING)
    #define TERMINUS_BEGIN_CPU_PROFILE(x) rmt_BeginCPUSample(x, 0);
    #define TERMINUS_END_CPU_PROFILE rmt_EndCPUSample();
    #define TERMINUS_PROFILER_INSTANCE Remotery* rmt;
    #define TERMINUS_CREATE_PROFILER rmt_CreateGlobalInstance(&rmt);
    #define TERMINUS_DESTROY_PROFILER rmt_DestroyGlobalInstance(rmt);
#else
    #define TERMINUS_BEGIN_CPU_PROFILE(x)
    #define TERMINUS_END_CPU_PROFILE
    #define TERMINUS_PROFILER_INSTANCE
    #define TERMINUS_CREATE_PROFILER
    #define TERMINUS_DESTROY_PROFILER
#endif

#define MAX_CONFIG_FIELDS		  20

#define CONFIG_APP_NAME			  "app_name"
#define CONFIG_APP_VERSION		  "app_version"
#define CONFIG_ENGINE_VERSION	  "engine_version"
#define CONFIG_ENGINE_BUILD		  "engine_build"
#define CONFIG_INITIAL_SCENE	  "initial_scene"
#define CONFIG_WINDOW_TITLE		  "window_title"
#define CONFIG_WINDOW_WIDTH		  "window_width"
#define CONFIG_WINDOW_RESIZABLE   "window_resizable"
#define CONFIG_WINDOW_HEIGHT	  "window_height"
#define CONFIG_WINDOW_MODE		  "window_mode"
#define CONFIG_WINDOW_REFRESH_RATE "window_refresh_rate"
#define CONFIG_WINDOW_VSYNC		  "window_vsync"
#define CONFIG_LOCAL_DATA_DIR	  "local_data_dir"
#define CONFIG_MESH_DIR			  "mesh_dir"
#define CONFIG_MATERIAL_DIR		  "material_dir"
#define CONFIG_TEXTURE_DIR		  "texture_dir"
#define CONFIG_SCRIPT_DIR		  "script_dir"
#define CONFIG_INPUT_MAP_DIR	  "input_map_dir"
#define CONFIG_RENDERING_PATH_DIR "rendering_path_dir"
#define CONFIG_ANIMATION_DIR	  "animation_dir"
#define CONFIG_SCENE_DIR		  "scene_dir"


