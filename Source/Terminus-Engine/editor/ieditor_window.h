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
                      Vector2 rel_size,
                      Vector2 size,
                      Vector2 rel_pos,
                      Vector2 pos,
                      int flags) : _is_open(false), _title(title), _rel_size(rel_size), _size(size), _rel_pos(rel_pos), _pos(pos), _flags(flags)
        {
            
        }
        
		virtual ~IEditorWindow(){}
        
        inline void update(double dt)
        {
            ImGui::SetNextWindowPos(ImVec2(_pos.x, _pos.y), ImGuiSetCond_Always);
            ImGui::SetNextWindowSize(ImVec2(_size.x, _size.y), ImGuiSetCond_Always);
            window_contents(dt);
        }
        
        inline void resize()
        {
            _size = imgui_helpers::get_size_from_ratio(_rel_size);
        }
        
        inline void repos()
        {
            
        }
        
        inline virtual void initialize()
        {
            resize();
        }
        
        inline bool is_open() { return _is_open; }
        inline void open() { _is_open = true; }
        inline void close() { _is_open = false; }
        inline void set_rel_size(Vector2 rel_size) { _rel_size = rel_size; }
        inline void set_size(Vector2 size) { _size = _size; }
        
    protected:
        virtual void window_contents(double dt) = 0;
        
    protected:
        bool _is_open;
        std::string _title;
        Vector2 _rel_size;
        Vector2 _size;
        
        Vector2 _rel_pos;
        Vector2 _pos;
        
        int _flags;
	};
}
