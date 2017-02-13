#include <editor/editor.h>
#include <string>

namespace terminus
{
	Editor::Editor()
	{
		_current_project = nullptr;
	}

	Editor::~Editor()
	{

	}

	void Editor::update(double dt)
	{

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

	void Editor::register_window(uint32_t id, IEditorWindow* window)
	{

	}

	void Editor::on_scene_load_complete(Event* event)
	{

	}
}