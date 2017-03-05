#pragma once

#ifndef TERMINUS_GLOBAL_H
#define TERMINUS_GLOBAL_H

#include <Memory/linear_allocator.h>
#include <Thread/thread_pool.h>
#include <IO/logger.h>
#include <core/config.h>

#define MB_IN_BYTES 1024*1024
#define MAX_MEMORY_MB 200
#define MAX_MEMORY MAX_MEMORY_MB*MB_IN_BYTES
#define MAX_PER_FRAME_MEMORY 100
#define MEMORY_ALIGNMENT 8
#define T_NEW new(__LINE__, __FILE__)

extern void* operator new (size_t size, unsigned line, const char* file);

namespace terminus
{
    using DefaultThreadPool = ThreadPool<0, 1000>;
	class Context;
    
    namespace global
    {
		extern Context* g_context;

		template<typename T>
		inline void initialize()
		{
			logger::initialize();

			logger::open_file_stream();

			g_memory = malloc(MAX_MEMORY);
			g_allocator = new LinearAllocator(MAX_MEMORY, g_memory);

			g_per_frame_memory = malloc(MAX_PER_FRAME_MEMORY);
			g_per_frame_allocator = new LinearAllocator(MAX_PER_FRAME_MEMORY, g_per_frame_memory);

			g_thread_pool = T_NEW DefaultThreadPool();

			g_context = T_NEW Context();
			g_context->_shutdown = false;
			g_context->_platform = T_NEW T();

			filesystem::add_directory("config");
			g_context->_engine_config = config_file_factory::create("engine_config.json");

#if defined(TERMINUS_WITH_EDITOR)
			imgui_console::initialize();
			logger::open_custom_stream(&imgui_console::logger_callback);
#endif
		}

        extern LinearAllocator* get_default_allocator();
        extern LinearAllocator* get_per_frame_allocator();
        extern DefaultThreadPool*   get_default_threadpool();
        extern void shutdown();
    }
}

#endif
