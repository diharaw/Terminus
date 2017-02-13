#include <Utility/imgui_helper.h>
#include <Core/context.h>

namespace imgui_helpers
{
    uint32_t get_size_from_ratio(float value)
    {
        if(value < 0.0f || value == 0.0f || value > 1.0f)
            value = 0.5f;
        float height = terminus::context::get_platform().get_height();
        
        return height * value;
    }
}
