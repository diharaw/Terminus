#include <core/application.h>
#include "platform/platform_sdl2.h"
#include "graphics/imgui_backend_sdl2.h"

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
            
            context._main_ready_sema.notify();
            context._load_wakeup_sema.notify();
            context._render_ready_sema.wait();
            
            ImGuiBackend* gui_backend = context::get_imgui_backend();
            
            // begin initial scene load here
#if !defined(TERMINUS_WITH_EDITOR)
            load_initial_scene();
#endif
            
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
                context._render_done_sema.wait();
                
                // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
                renderer.swap();
                
                TERMINUS_BEGIN_CPU_PROFILE(gui_update);
                //temp_render();
                gui_backend->new_frame();
                imgui_console::draw();
                
                global::get_per_frame_allocator()->Clear();
                
                context._swap_done_sema.notify();
                platform->end_frame();
                
                TERMINUS_END_CPU_PROFILE;
            }
        }

        void shutdown() override
        {
            Context& context = global::get_context();
            
            physics::shutdown();
            
            context._load_wakeup_sema.notify();
            context._load_exit_sema.wait();
            context._render_exit_sema.wait();
            
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

int main()
{
    terminus::global::initialize();                                 
    terminus::global::set_platform(T_NEW terminus::PlatformSDL2());
    terminus::global::set_imgui_backend(T_NEW terminus::ImGuiBackendSDL2());
    terminus::Application* app = T_NEW terminus::RuntimeApp();
    
    if(app->initialize())											
    {																
        app->run();													
    }
    
    app->shutdown();												
    terminus::global::shutdown();
    
    return 0;
}
