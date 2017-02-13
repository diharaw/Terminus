#pragma once

#include <imgui.h>
#include <string>
#include <Utility/imgui_helper.h>

namespace terminus
{
	class IEditorWindow
	{
	public:
        IEditorWindow(std::string title,
                      float rel_width,
                      float rel_height) : _is_open(true), _title(title), _rel_height(rel_height), _rel_width(rel_width) {}
		virtual ~IEditorWindow(){}
        
        inline void update(double dt)
        {
            ImGui::SetNextWindowSize(ImVec2(imgui_helpers::get_size_from_ratio(_rel_width),
                                            imgui_helpers::get_size_from_ratio(_rel_height)),
                                     ImGuiSetCond_FirstUseEver);
            
            ImGui::Begin(_title.c_str(), &_is_open, 0);
            
            window_contents(dt);
            
            ImGui::End();
        }
        
        inline bool is_open() { return _is_open; }
        inline void open() { _is_open = true; }
        inline void close() { _is_open = false; }
        inline void set_height(float height) { _rel_height = height; }
        inline void set_width(float width) { _rel_width = width; }
        
    protected:
        virtual void window_contents(double dt) = 0;
        
    protected:
        bool _is_open;
        std::string _title;
        float _rel_height;
        float _rel_width;
	};
}
