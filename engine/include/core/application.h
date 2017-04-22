#pragma once

#include <core/config.h>

#if defined(TERMINUS_PLATFORM_WIN32)

#define TERMINUS_DELCARE_MAIN(AppClass)                                                             \
                                                                                                    \
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)     \
{                                                                                                   \
    terminus::global::initialize();                                                                 \
    terminus::global::set_platform(T_NEW terminus::PlatformSDL2());                                 \
    terminus::global::set_imgui_backend(T_NEW terminus::ImGuiBackendSDL2());                        \
    terminus::Application* app = T_NEW AppClass();                                                  \
                                                                                                    \
    if (app->initialize())                                                                          \
    {                                                                                               \
        app->run();                                                                                 \
    }                                                                                               \
                                                                                                    \
    app->shutdown();                                                                                \
    terminus::global::shutdown();                                                                   \
                                                                                                    \
    return 0;                                                                                       \
}                                                                                                   \

#else

#define TERMINUS_DELCARE_MAIN(AppClass)                                                             \
                                                                                                    \
int main()                                                                                          \
{                                                                                                   \
    terminus::global::initialize();                                                                 \
    terminus::global::set_platform(T_NEW terminus::PlatformSDL2());                                 \
    terminus::global::set_imgui_backend(T_NEW terminus::ImGuiBackendSDL2());                        \
    terminus::Application* app = T_NEW AppClass();                                                  \
                                                                                                    \
    if (app->initialize())                                                                          \
    {                                                                                               \
        app->run();                                                                                 \
    }                                                                                               \
                                                                                                    \
    app->shutdown();                                                                                \
    terminus::global::shutdown();                                                                   \
                                                                                                    \
    return 0;                                                                                       \
}

#endif


namespace terminus
{
    class ThreadPool;
    
	class Application
	{
	protected:
		ThreadPool*   _main_thread_pool;
        
	public:
        Application();
        virtual ~Application();
        
        bool initialize();
        void shutdown();
        
    private:
        bool initialize_engine();
        void shutdown_engine();
        
        void initialize_input();
        void initialize_resources();
        void initialize_physics();
        void initialize_audio();
        void initialize_ecs();
        void initialize_script();
        
    public:
        virtual void run() = 0;
        
    protected:
        virtual bool initialize_app() = 0;
        virtual void shutdown_app() = 0;
	};
}
