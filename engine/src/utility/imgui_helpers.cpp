#include <utility/imgui_helper.h>
#include <core/context.h>

namespace imgui_helpers
{
    uint32_t get_size_from_ratio(float value)
    {
        if(value < 0.0f || value == 0.0f || value > 1.0f)
            value = 0.5f;
        float height = terminus::context::get_platform()->get_height();
        
        return height * value;
    }
    
    uint32_t get_pos_x_from_rel(float value)
    {
        if(value < 0.0f || value > 1.0f)
            value = 0.0f;
        float width = terminus::context::get_platform()->get_width();
        
        return width * value;
    }
    
    uint32_t get_pos_y_from_rel(float value)
    {
        if(value < 0.0f || value > 1.0f)
            value = 0.0f;
        float height = terminus::context::get_platform()->get_height();
        
        return height * value;
    }
    
    Vector2 get_size_from_ratio(Vector2 ratio)
    {
        float height = terminus::context::get_platform()->get_height();
        float width = terminus::context::get_platform()->get_width();
        
        if(ratio.x > 1.0f || ratio.x == 0.0f || ratio.x < 0.0f)
            ratio.x = 0.0f;
        
        if(ratio.y > 1.0f || ratio.y == 0.0f || ratio.y < 0.0f)
            ratio.y = 0.0f;
        
        Vector2 size;
        size.x = width * ratio.x;
        size.y = height * ratio.y;
        
        return size;
    }
    
    Vector2 get_pos_from_rel(Vector2 rel_pos)
    {
        float height = terminus::context::get_platform()->get_height();
        float width = terminus::context::get_platform()->get_width();
        
        if(rel_pos.x > 1.0f || rel_pos.x < 0.0f)
            rel_pos.x = 0.0f;
        
        if(rel_pos.y > 1.0f || rel_pos.y < 0.0f)
            rel_pos.y = 0.0f;
        
        Vector2 pos;
        pos.x = width * rel_pos.x;
        pos.y = height * rel_pos.y;
        
        return pos;
    }
    
    Vector2 get_pos_from_rel_center(Vector2 rel_pos, Vector2 size)
    {
        float height = terminus::context::get_platform()->get_height();
        float width = terminus::context::get_platform()->get_width();
        
        if(rel_pos.x > 1.0f || rel_pos.x < 0.0f)
            rel_pos.x = 0.0f;
        
        if(rel_pos.y > 1.0f || rel_pos.y < 0.0f)
            rel_pos.y = 0.0f;
        
        Vector2 pos;
        pos.x = (width * rel_pos.x) - (size.x / 2.0f);
        pos.y = (height * rel_pos.y) - (size.y / 2.0f) ;
        
        return pos;
    }
}
