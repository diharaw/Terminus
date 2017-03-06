#pragma once

#include <SDL.h>
#include <graphics/imgui_backend.h>

namespace terminus
{
    namespace imgui_backend_sdl2
    {
        extern void set_window(SDL_Window* window);
        extern bool process_window_events(SDL_Event* event);
    }
    
    class ImGuiBackendSDL2 : public ImGuiBackend
    {
    public:
        ImGuiBackendSDL2();
        virtual ~ImGuiBackendSDL2();
        void initialize() override;
        void new_frame() override;
        void shutdown() override;
        void render() override;
    };
}
