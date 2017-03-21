#include <core/application.h>
#include <core/sync.h>
#include "platform/platform_sdl2.h"
#include "graphics/imgui_backend_sdl2.h"

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#endif

namespace terminus
{
    TERMINUS_PROFILER_INSTANCE
    
    class RuntimeApp : public Application
    {
    public:
        RuntimeApp()
        {
            
        }
        
        ~RuntimeApp()
        {
            
        }
        
        bool initialize() override
        {
            _main_thread_pool = global::get_default_threadpool();
            
            TERMINUS_CREATE_PROFILER
            
            if (!context::get_platform()->initialize())
                return false;
            
            initialize_input();
            initialize_resources();
            initialize_ecs();
            initialize_physics();
            initialize_audio();
            initialize_script();
            
            return true;
        }
        
        void run() override
        {
            Context& context = global::get_context();
            Platform* platform = context::get_platform();
            Renderer& renderer = context::get_renderer();
            
            context._rendering_thread.run();
            context._loading_thread.run();
            
			sync::notify_main_ready();
			sync::notify_loader_wakeup();
			sync::wait_for_renderer_ready();
            
            ImGuiBackend* gui_backend = context::get_imgui_backend();
            
            // begin initial scene load here
            load_initial_scene();
            
            while (!context._shutdown)
            {
                TERMINUS_BEGIN_CPU_PROFILE(simulation);
                
                platform->begin_frame();
                platform->update();
                EventHandler::update();
                physics::update(platform->get_delta_time());
                
                context._scene_manager.update(platform->get_delta_time());
                
                TERMINUS_END_CPU_PROFILE;
                // Synchronize Rendering Thread
				sync::wait_for_renderer_done();
                
                // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
                renderer.swap();
                
                TERMINUS_BEGIN_CPU_PROFILE(gui_update);
                //temp_render();
                gui_backend->new_frame();
                imgui_console::draw();
				static bool tw = true;
				ImGui::ShowTestWindow(&tw);
                
                global::get_per_frame_allocator()->Clear();
                
				sync::notify_swap_done();
                platform->end_frame();
                
                TERMINUS_END_CPU_PROFILE;
            }
        }

        void shutdown() override
        {
            Context& context = global::get_context();
            
            physics::shutdown();
            
			sync::notify_loader_wakeup();
			sync::wait_for_loader_exit();
			sync::wait_for_renderer_exit();
            
            context._loading_thread.exit();
            context._rendering_thread.exit();
            
            context::get_dynamic_library_cache().shutdown();
            context::get_platform()->shutdown();
            
            TERMINUS_DESTROY_PROFILER
        }
        
        
    private:
        void load_initial_scene()
        {
            SceneManager& scene_manager = context::get_scene_manager();
            
            if (context::get_engine_config()->has_value(CONFIG_INITIAL_SCENE))
            {
                StringBuffer64 scene_name = context::get_engine_config()->get_string(CONFIG_INITIAL_SCENE);
                scene_manager.load(String(scene_name.c_str()));
            }
        }
        
        void initialize_input()
        {
            input_handler::initialize();
        }
        
        void initialize_resources()
        {
            context::get_scene_cache().initialize();
        }
        
        void initialize_physics()
        {
            physics::initialize();
        }
        
        void initialize_audio()
        {
            
        }
        
        void initialize_ecs()
        {
            context::get_scene_manager().initialize();
        }
        
        void initialize_script()
        {
            context::get_script_engine().initialize();
        }
    };
}

#if defined(TERMINUS_PLATFORM_WIN32)

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	terminus::global::initialize();
	terminus::global::set_platform(T_NEW terminus::PlatformSDL2());
	terminus::global::set_imgui_backend(T_NEW terminus::ImGuiBackendSDL2());
	terminus::Application* app = T_NEW terminus::RuntimeApp();

	if (app->initialize())
	{
		app->run();
	}

	app->shutdown();
	terminus::global::shutdown();

	return 0;
}

#else
	
int main()
{
	terminus::global::initialize();
	terminus::global::set_platform(T_NEW terminus::PlatformSDL2());
	terminus::global::set_imgui_backend(T_NEW terminus::ImGuiBackendSDL2());
	terminus::Application* app = T_NEW terminus::RuntimeApp();

	if (app->initialize())
	{
		app->run();
	}

	app->shutdown();
	terminus::global::shutdown();

	return 0;
}

#endif


