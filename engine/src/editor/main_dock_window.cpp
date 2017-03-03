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
        _selected_entity = -1;
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
        ImGui::ShowTestWindow();
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
                Scene* active_scene = context::get_editor().get_current_scene();
                
                if(active_scene)
                {
                    uint32_t num_entities = active_scene->get_num_entities();
                    
                    for(int i = 0; i < num_entities; i++)
                    {
                        ImGuiTreeNodeFlags flags = 0;
                        
                        if(i == _selected_entity)
                            flags = ImGuiTreeNodeFlags_Selected;
                        
                        ImGui::TreeNodeEx((void*)(intptr_t)i, flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", active_scene->_entities._objects[i]._name.c_str());
                        if (ImGui::IsItemClicked())
                            _selected_entity = i;
                    }
                }
            }
            ImGui::End();
        }
    }
    
    void MainDockWindow::inspector_window()
    {
        if(_inspector)
        {
            bool transform_cmp = false;
            bool mesh_cmp = false;
            bool camera_cmp = false;
            bool lua_script_cmp = false;
            bool cpp_script_cmp = false;
            
            if(ImGui::Begin("Inspector", &_inspector, 0))
            {
                if(_selected_entity != -1)
                {
                    Scene* active_scene = context::get_editor().get_current_scene();
                    
                    Entity& entity = active_scene->_entities._objects[_selected_entity];
                    ImGui::InputText("Entity Name", (char*)entity._name.c_str(), entity._name.size());
                    
                    ImGui::Separator();
                    
                    // transform
                    
                    if(active_scene->has_transform_component(entity))
                    {
                        transform_cmp = true;
                        if (ImGui::CollapsingHeader("Transform"))
                        {
                            TransformComponent& cmp = active_scene->get_transform_component(entity);
                            
                            ImGui::InputFloat3("Position", &cmp._position.x);
                            ImGui::InputFloat3("Rotation", &cmp._rotation.x);
                            ImGui::InputFloat3("Scale", &cmp._scale.x);
                            
                            cmp._is_dirty = true;
                        }
                    }
                    
                    // mesh
                    
                    if(active_scene->has_mesh_component(entity))
                    {
                        mesh_cmp = true;
                        if (ImGui::CollapsingHeader("Mesh"))
                        {
                            MeshComponent& cmp = active_scene->get_mesh_component(entity);
                            
                            ImGui::InputText("Mesh File", (char*)cmp.mesh_name.c_str(), cmp.mesh_name.size());
                            ImGui::SameLine();
                            
                            if(ImGui::Button("Load"))
                            {
                                
                            }
                        }
                    }
                    
                    // camera
                    
                    if(active_scene->has_camera_component(entity))
                    {
                        camera_cmp = true;
                        if (ImGui::CollapsingHeader("Camera"))
                        {
                            CameraComponent& cmp = active_scene->get_camera_component(entity);
                            
                            ImGui::InputFloat3("Position", &cmp.transform._position.x);
                            ImGui::InputFloat3("Rotation", &cmp.transform._rotation.x);
                            ImGui::InputFloat3("Scale", &cmp.transform._scale.x);
                            ImGui::Spacing();
                            ImGui::InputText("Rendering Path", (char*)cmp.rendering_path->_name.c_str(), cmp.rendering_path->_name.size());
                            ImGui::SameLine();
                            
                            if(ImGui::Button("Load"))
                            {
                                
                            }
                        }
                    }
                    
                    // lua script
                    
                    if(active_scene->has_lua_script_component(entity))
                    {
                        lua_script_cmp = true;
                        if (ImGui::CollapsingHeader("Lua Script"))
                        {
                            LuaScriptComponent& cmp = active_scene->get_lua_script_component(entity);
                            
                            ImGui::InputText("Script", (char*)cmp._script->_script_name.c_str(), cmp._script->_script_name.size());
                        }
                    }
                    
                    // cpp script
                    
                    if(active_scene->has_cpp_script_component(entity))
                    {
                        cpp_script_cmp = true;
                        if (ImGui::CollapsingHeader("C++ Script"))
                        {
                            CppScriptComponent& cmp = active_scene->get_cpp_script_component(entity);
                            ImGui::SameLine();
                            
                            if(ImGui::Button("Load"))
                            {
                                
                            }
                        }
                    }
                    
                    // add component
                    
                    if(ImGui::Button("New Component", ImVec2(ImGui::GetContentRegionAvailWidth(), 30.0f)))
                    {
                       ImGui::OpenPopup("cmp_list");
                    }
                    
                    if (ImGui::BeginPopup("cmp_list"))
                    {
                        ImGui::Separator();
                    
                        if(!transform_cmp)
                        {
                            if (ImGui::Selectable("Transform"))
                            {
                                
                            }
                        }
                        if(!mesh_cmp)
                        {
                            if (ImGui::Selectable("Mesh"))
                            {
                                
                            }
                        }
                        if(!camera_cmp)
                        {
                            if (ImGui::Selectable("Camera"))
                            {
                                
                            }
                        }
                        if(!lua_script_cmp)
                        {
                            if (ImGui::Selectable("Lua Script"))
                            {
                                
                            }
                        }
                        if(!cpp_script_cmp)
                        {
                            if (ImGui::Selectable("C++ Script"))
                            {
                                
                            }
                        }
                        
                        ImGui::EndPopup();
                    }
                }
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
