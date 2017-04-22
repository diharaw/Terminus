#include "editor.h"
#include "project.h"
#include <gameplay/scene.h>
#include <core/global.h>
#include <core/context.h>
#include <graphics/imgui_console.h>

#include <imgui_dock.h>

namespace terminus
{
	bool		   _play_mode	 = false;
	Scene*		   _active_scene = nullptr;
	Scene		   _play_mode_scene;
	ImGuiBackend*  _gui_backend = nullptr;
	Project*	   _active_project = nullptr;
	nlohmann::json _scene_json;
	nlohmann::json _editor_config;

	namespace editor
	{
		static bool show_scene1 = true;
		static bool show_scene2 = true;
		static bool show_scene3 = true;
		static bool show_scene4 = true;
		static bool show_scene5 = true;
		static bool show_scene6 = true;
		static bool show_scene7 = true;

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
					if (ImGui::MenuItem("Asset Browser"))
						show_scene7 = !show_scene7;
					if (ImGui::MenuItem("Log"))
						show_scene6 = !show_scene6;
					if (ImGui::MenuItem("Scene Heirachy"))
						show_scene4 = !show_scene4;
					if (ImGui::MenuItem("Profiler"))
						show_scene5 = !show_scene5;
					if (ImGui::MenuItem("Inspector"))
						show_scene3 = !show_scene3;
					if (ImGui::MenuItem("Game View"))
						show_scene2 = !show_scene2;
					if (ImGui::MenuItem("Edit View"))
						show_scene1 = !show_scene1;
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
		}

		void shutdown() 
		{
			ImGui::SaveDock();
		}

		void update()
		{
			_gui_backend->new_frame();

			int menu_height = main_menu_GUI();

			auto pos = ImVec2(0, menu_height);
			auto size = ImGui::GetIO().DisplaySize;
			size.y -= pos.y;
			ImGui::RootDock(pos, ImVec2(size.x, size.y));

			// Middle

			if (ImGui::BeginDock("Edit View", &show_scene1)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Game View", &show_scene2)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();

			// Sides

			if (ImGui::BeginDock("Inspector", &show_scene3)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Scene Heirachy", &show_scene4)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();

			// Bottom

			if (ImGui::BeginDock("Profiler", &show_scene5)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Log", &show_scene6)) {
				imgui_console::draw();
			}
			ImGui::EndDock();

			if (ImGui::BeginDock("Asset Browser", &show_scene7)) {
				ImGui::Text("Placeholder!");
			}
			ImGui::EndDock();
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

		}

		void load_scene(std::string scene)
		{

		}
	}
}