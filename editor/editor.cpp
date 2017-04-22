#include "editor.h"
#include "project.h"
#include <gameplay/scene.h>
#include <core/global.h>
#include <core/context.h>
#include <resource/text_loader.h>
#include <graphics/imgui_console.h>
#include <imgui.h>
#include <imgui_dock.h>

#include "asset_browser_dock.h"
#include "inspector_dock.h"
#include "scene_heirachy_dock.h"
#include "scene_viewport_dock.h"
#include "profiler_dock.h"
#include "logger_dock.h"

#include <json.hpp>

namespace terminus
{
	bool		   _play_mode	 = false;
	Scene*		   _active_scene = nullptr;
	Scene		   _play_mode_scene;
	ImGuiBackend*  _gui_backend = nullptr;
	Project*	   _active_project = nullptr;
	nlohmann::json _scene_json;
	nlohmann::json _editor_config;
	nlohmann::json _project_json;
	StringBuffer32 _project_name;

	// Docks
	AssetBrowserDock  _asset_browser_dock;
	InspectorDock	  _inspector_dock;
	LoggerDock		  _logger_dock;
	SceneHeirachyDock _scene_heirachy_dock;
	SceneViewportDock _scene_viewport_dock;
	ProfilerDock	  _profiler_dock;

	namespace editor
	{
		struct SceneLoadData
		{
			char scene_name[100];
		};

		void scene_load_task(void* data)
		{
			SceneLoadData* load_data = (SceneLoadData*)data;
			Scene* loaded = context::get_scene_cache().load(String(load_data->scene_name));

			// Fire Scene Load Complete Event

			SceneLoadEvent* event = new SceneLoadEvent(loaded);
			EventHandler::queue_event(event);
		}

		void on_scene_load_complete(Event* data)
		{
			SceneLoadEvent* event_data = (SceneLoadEvent*)data;
			_active_scene = event_data->GetScene();
		}

		int main_menu_GUI() 
		{
			//////////////////////
			// Placeholder menu //
			//////////////////////
			int menu_height = 0;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::BeginMenu("Open Recent"))
					{
						ImGui::MenuItem("fish_hat.c");
						ImGui::MenuItem("fish_hat.inl");
						ImGui::MenuItem("fish_hat.h");
						if (ImGui::BeginMenu("More.."))
						{
							ImGui::MenuItem("Hello");
							ImGui::MenuItem("Sailor");
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Disabled", false)) // Disabled
					{
						IM_ASSERT(0);
					}
					if (ImGui::MenuItem("Quit", "Alt+F4")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "CTRL+X")) {}
					if (ImGui::MenuItem("Copy", "CTRL+C")) {}
					if (ImGui::MenuItem("Paste", "CTRL+V")) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View"))
				{
					ImGui::EndMenu();
				}
				menu_height = ImGui::GetWindowSize().y;

				ImGui::EndMainMenuBar();
			}

			return menu_height;
		}

		void initialize()
		{
			_gui_backend = context::get_imgui_backend();
			ImGui::LoadDock();

			_asset_browser_dock.set_title("Asset Browser");
			_inspector_dock.set_title("Inspector");
			_logger_dock.set_title("Log");
			_scene_heirachy_dock.set_title("Scene Heirachy");
			_scene_viewport_dock.set_title("Viewport");
			_profiler_dock.set_title("Profiler");

			EventCallback callback;
			callback.Bind<&on_scene_load_complete>();
			EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);

			load_config();
		}

		void shutdown() 
		{
			ImGui::SaveDock();
		}

		void update(double dt)
		{
			_gui_backend->new_frame();

			int menu_height = main_menu_GUI();

			auto pos = ImVec2(0, menu_height);
			auto size = ImGui::GetIO().DisplaySize;
			size.y -= pos.y;

			ImGui::RootDock(pos, ImVec2(size.x, size.y));

			_asset_browser_dock.update(dt);
			_inspector_dock.update(dt);
			_logger_dock.update(dt);
			_scene_heirachy_dock.update(dt);
			_scene_viewport_dock.update(dt);
			_profiler_dock.update(dt);
		}

		void update_play_mode(double delta)
		{
			_play_mode_scene.update(delta);
		}

		void enter_play_mode()
		{
			memcpy(&_play_mode_scene, _active_scene, sizeof(Scene));
			_play_mode = true;
		}

		Scene* active_scene()
		{
			return &_play_mode_scene;
		}

		void exit_play_mode()
		{
			_play_mode = false;
		}

		bool play_mode()
		{
			return _play_mode;
		}

		void load_project(std::string project)
		{
			FileHandle file = filesystem::read_file(project, true, true);
			_project_json = nlohmann::json::parse(file.buffer);

			std::string name = _project_json["project_name"];
			_project_name = name;

			nlohmann::json scenes = _project_json["scenes"];

			if (scenes.size() > 0)
			{
				std::string scene = scenes[0];
				load_scene(scene);
			}

			free(file.buffer);
		}

		void load_scene(std::string scene)
		{
			Task task;
			LoadingThread& loading_thread = global::get_context()._loading_thread;

			SceneLoadData* data = task_data<SceneLoadData>(task);

			const char* name = scene.c_str();
			strcpy(&data->scene_name[0], name);

			task._function.Bind<&scene_load_task>();
			loading_thread.enqueue_load_task(task);

			free(data);
		}

		void load_config()
		{
			asset_common::TextLoadData* load_data = text_loader::load("editor_config.json");
			_editor_config = nlohmann::json::parse(load_data->buffer);

			nlohmann::json recent_project = _editor_config["recent_projects"];

			if (recent_project.size() > 0)
			{
				std::string project = recent_project[0];
				load_project(project);
			}

			free(load_data);
		}

		void create_scene(std::string name)
		{
			T_SAFE_DELETE(_active_scene);
			_active_scene = new Scene(name);

			// Add new scene to current project
			_project_json["scenes"].push_back(name);

			// Save scene
			std::string path = "assets/scene/";
			path += name;
			path += ".json";

			if (filesystem::write_begin(path))
			{
				std::string config = _editor_config.dump();
				filesystem::write((void*)config.c_str(), config.size(), 1, 0);

				filesystem::write_end();
			}
		}

		void create_project(std::string name)
		{
			// Add new project to recent projects in Config
			_editor_config["recent_projects"].push_back(name);

			// Save config
			if (filesystem::write_begin("assets/editor_config.json"))
			{
				std::string config = _editor_config.dump();
				filesystem::write((void*)config.c_str(), config.size(), 1, 0);

				filesystem::write_end();
			}
		}
	}
}