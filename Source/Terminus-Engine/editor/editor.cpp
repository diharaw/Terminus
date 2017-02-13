#include <editor/editor.h>
#include <editor/project_selection_window.h>
#include <string>

namespace terminus
{
	Editor::Editor()
	{
		_current_project = nullptr;
        
        register_window(0, new ProjectSelectionWindow("Select Project", 0.5f, 0.6f));
        open_window(0);
	}

	Editor::~Editor()
	{

	}

	void Editor::update(double dt)
	{
        for(auto itr : _window_map)
        {
            if(itr.second->is_open())
                itr.second->update(dt);
        }
	}

	void Editor::create_project(const char* name, const char* path)
	{
		_current_project = new Project();
		_current_project->create(name, path);
	}

	void Editor::load_project(const char* name)
	{
		_current_project = new Project();

		
	}
    
    void Editor::open_window(uint32_t id)
    {
        if(_window_map.find(id) != _window_map.end())
        {
            _window_map[id]->open();
        }
    }

	void Editor::register_window(uint32_t id, IEditorWindow* window)
	{
        _window_map[id] = window;
	}

	void Editor::on_scene_load_complete(Event* event)
	{

	}
}
