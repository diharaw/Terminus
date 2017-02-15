#include <editor/main_dock_window.h>
#include <core/context.h>
#include <graphics/imgui_console.h>

#include <imgui_dock.h>

namespace terminus
{
    MainDockWindow::MainDockWindow(std::string title,
                                   Vector2 rel_size,
                                   Vector2 size,
                                   Vector2 rel_pos,
                                   Vector2 pos,
                                   int flags) : IEditorWindow(title, rel_size, size, rel_pos, pos, flags)
    {
        _exit_modal = false;
        _use_docks = false;
        
        if(_use_docks)
        {
            _editor_viewport = true;
            _profiler = true;
            _asset_browser = true;
            _log = true;
            _inspector = true;
            _scene_heirachy = true;
            _game_viewport = true;
        }
        else
        {
            _editor_viewport = false;
            _profiler = false;
            _asset_browser = false;
            _log = false;
            _inspector = false;
            _scene_heirachy = false;
            _game_viewport = false;
        }
    }
    
    MainDockWindow::~MainDockWindow()
    {
        ImGui::ShutdownDock();
    }
    
    int MainDockWindow::main_menu_bar()
    {
        int menu_height = 0;
        
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                file_menu();
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edt"))
            {
                edit_menu();
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View"))
            {
                view_menu();
                ImGui::EndMenu();
            }
            
            menu_height = ImGui::GetWindowSize().y;
            
            ImGui::EndMainMenuBar();
        }
        
        return menu_height;
    }
    
    void MainDockWindow::window_contents(double dt)
    {
        main_menu_bar();
        
        if(_exit_modal)
            ImGui::OpenPopup("Exit Editor");
        
        exit_modal();

        if(_use_docks)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0);
            
            ImGui::SetNextWindowSize(ImVec2(_size.x, _size.y - 20.0f), ImGuiSetCond_FirstUseEver);
            ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
            
            ImGui::Begin(_title.c_str(), &_is_open, _flags);
            
            dock_contents();
            
            ImGui::End();
            
            ImGui::PopStyleVar();
        }
        else
        {
            scene_window();
            inspector_window();
            log_window();
            asset_browser_window();
        }
    }
    
    void MainDockWindow::scene_window()
    {
        if(_scene_heirachy)
        {
            if(ImGui::Begin("Scene", &_scene_heirachy, 0))
            {
                ImGui::Text("Hello from win!");
            }
            ImGui::End();
        }
    }
    
    void MainDockWindow::inspector_window()
    {
        if(_inspector)
        {
            if(ImGui::Begin("Inspector", &_inspector, 0))
            {
                ImGui::Text("Hello from win!");
            }
            ImGui::End();
        }
    }
    
    void MainDockWindow::log_window()
    {
        if(_log)
        {
            if(ImGui::Begin("Log", &_log, 0))
            {
                imgui_console::draw();
            }
            ImGui::End();
        }
    }
    
    void MainDockWindow::asset_browser_window()
    {
        if(_asset_browser)
        {
            if(ImGui::Begin("Asset Browser", &_asset_browser, 0))
            {
                ImGui::Text("Hello from win!");
            }
            ImGui::End();
        }
    }
    
    void MainDockWindow::scene_heirachy_contents()
    {
        
    }
    
    void MainDockWindow::inspector_contents()
    {
        
    }
    
    void MainDockWindow::dock_contents()
    {
        ImGui::BeginDockspace();
        ImGui::SetNextDock(ImGuiDockSlot_Bottom);
        if (ImGui::BeginDock("Asset Browser", &_asset_browser))
        {
            //ImGui::SetCurrentDockInitialPosSize(ImVec2(0.0f, 0.0f), ImVec2((float)Terminus::Window::GetWidth(), m_BottomDockSize.y));
            //AssetBrowser();
            ImGui::Text("Hellloooo");
        }
        ImGui::EndDock();
        
        if (ImGui::BeginDock("Profiler", &_profiler))
        {
            //ImGui::SetCurrentDockInitialPosSize(ImVec2(0.0f, 0.0f), ImVec2((float)Terminus::Window::GetWidth(), m_BottomDockSize.y));
            ImGui::Text("Hellloooo");
        }
        ImGui::EndDock();
        
        if (ImGui::BeginDock("Log", &_log))
        {
            //ImGui::SetCurrentDockInitialPosSize(ImVec2(0.0f, 0.0f), ImVec2((float)Terminus::Window::GetWidth(), m_BottomDockSize.y));
            imgui_console::draw();
        }
        ImGui::EndDock();
        
        ImGui::SetNextDock(ImGuiDockSlot_Top);
        if (ImGui::BeginDock("Inspector", &_inspector))
        {
            //ImGui::SetCurrentDockInitialPosSize(ImVec2(m_ViewportSize.x + m_SidePanelWidth, 0.0f), ImVec2(m_SidePanelWidth, m_ViewportSize.y));
            ImGui::Text("Hellloooo");
        }
        ImGui::EndDock();
        
        ImGui::SetNextDock(ImGuiDockSlot_Left);
        if (ImGui::BeginDock("Scene", &_scene_heirachy))
        {
            //ImGui::SetCurrentDockInitialPosSize(ImVec2(0.0f, 0.0f), ImVec2(m_SidePanelWidth, m_ViewportSize.y));
            scene_heirachy_contents();
        }
        ImGui::EndDock();
        
        ImGui::SetNextDock(ImGuiDockSlot_Right);
        if (ImGui::BeginDock("Viewport", &_editor_viewport))
        {
        }
        ImGui::EndDock();
        
        ImGui::EndDockspace();
    }
    
    void MainDockWindow::file_menu()
    {
        if (ImGui::BeginMenu("New"))
        {
            if (ImGui::MenuItem("Entity"))
            {
                
            }
            
            if (ImGui::MenuItem("Scene"))
            {
                
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::MenuItem("Open"))
        {
            
        }
        
        if (ImGui::MenuItem("Exit", "Esc"))
        {
            _exit_modal = true;
        }
    }
    
    void MainDockWindow::exit_modal()
    {
        if (ImGui::BeginPopupModal("Exit Editor", NULL , ImGuiWindowFlags_AlwaysAutoResize))
        {
            _exit_modal = false;
            ImGui::Text("Are you sure you want to exit the Editor?");
            
            if (ImGui::Button("Yes", ImVec2(120, 0)))
                context::get_platform().request_shutdown();
            
            ImGui::SameLine();
            
            if (ImGui::Button("No", ImVec2(120, 0)))
                ImGui::CloseCurrentPopup();
            
            ImGui::EndPopup();
        }
    }
    
    void MainDockWindow::view_menu()
    {
        ImGui::MenuItem("Asset Browser", NULL, &_asset_browser);
        ImGui::MenuItem("Log", NULL, &_log);
        ImGui::MenuItem("Profiler", NULL, &_profiler);
        ImGui::MenuItem("Inspector", NULL, &_inspector);
        ImGui::MenuItem("Scene Heirachy", NULL, &_scene_heirachy);
        ImGui::MenuItem("Viewport", NULL, &_editor_viewport);
    }
    
    void MainDockWindow::edit_menu()
    {
        
    }
    
    void MainDockWindow::initialize()
    {
        resize();
        _flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
    }
}