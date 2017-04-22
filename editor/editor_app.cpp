#include "editor_app.h"
#include "editor.h"

#include <core/terminus.h>
#include <core/sync.h>
#include <platform/platform_sdl2.h>
#include <graphics/imgui_backend_sdl2.h>

#if defined(TERMINUS_PLATFORM_WIN32)
#include <Windows.h>
#endif

namespace terminus
{
    EditorApp::EditorApp()
    {

    }
    
    EditorApp::~EditorApp()
    {
        
    }
    
    void EditorApp::run()
    {
        Context& context = global::get_context();
        Platform* platform = context::get_platform();
        Renderer& renderer = context::get_renderer();
        
        while (!context._shutdown)
        {
            
            
            platform->begin_frame();
            platform->update();
            EventHandler::update();

			if (editor::play_mode())
			{
				TERMINUS_BEGIN_CPU_PROFILE(simulation);

				// Update Scene
				physics::update(platform->get_delta_time());
				editor::update_play_mode(platform->get_delta_time());

				TERMINUS_END_CPU_PROFILE;

				TERMINUS_BEGIN_CPU_PROFILE(command_generation_play);

				// Render Scene
				renderer.generate_commands(editor::active_scene());

				TERMINUS_END_CPU_PROFILE;
			}
			else
			{
				TERMINUS_BEGIN_CPU_PROFILE(command_generation_edit);

				TERMINUS_END_CPU_PROFILE;
			}

            
            // Synchronize Rendering Thread
            sync::wait_for_renderer_done();
            
            // Only swap Graphics Queues when Front-Buffer Command generation and Back-Buffer Command Submission has completed.
            renderer.swap();
            
			editor::update();
            
            global::get_per_frame_allocator()->Clear();
            
            sync::notify_swap_done();
            platform->end_frame();
        }
        
    }
    
    bool EditorApp::initialize_app()
    {
		editor::initialize();
        return true;
    }
    
    void EditorApp::shutdown_app()
    {
		editor::shutdown();
    }
}
