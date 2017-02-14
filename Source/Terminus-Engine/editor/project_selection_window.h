#pragma once

#include <editor/ieditor_window.h>
#include <Core/Event/event.h>

namespace terminus
{
	class ProjectSelectionWindow : public IEditorWindow
	{
	public:
        ProjectSelectionWindow(std::string title, Vector2 rel_size, Vector2 size, Vector2 rel_pos, Vector2 pos, int flags);
		virtual ~ProjectSelectionWindow();
		virtual void window_contents(double dt) override;
        virtual void initialize() override;
        void on_scene_load_complete(Event* event);
        
    private:
        std::string _project_path;
        int _listener_id;
	};
}
