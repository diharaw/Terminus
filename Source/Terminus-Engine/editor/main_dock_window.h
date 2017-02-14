#pragma once

#include <editor/ieditor_window.h>
#include <types.h>

namespace terminus
{
    class MainDockWindow : public IEditorWindow
    {
    public:
        MainDockWindow(std::string title, Vector2 rel_size, Vector2 size, Vector2 rel_pos, Vector2 pos, int flags);
        virtual ~MainDockWindow();
        virtual void window_contents(double dt) override;
        virtual void initialize() override;
        void file_menu();
        void view_menu();
        void edit_menu();
        void exit_modal();
        void dock_contents();
        void scene_heirachy_contents();
        void inspector_contents();
        
    private:
        bool _editor_viewport;
        bool _game_viewport;
        bool _scene_heirachy;
        bool _profiler;
        bool _asset_browser;
        bool _log;
        bool _inspector;
        bool _exit_modal;
    };
}
