#pragma once

#include <stdint.h>
#include <core/types.h>

namespace imgui_helpers
{
    extern uint32_t get_size_from_ratio(float value);
    extern uint32_t get_pos_x_from_rel(float value);
    extern uint32_t get_pos_y_from_rel(float value);
    extern Vector2 get_size_from_ratio(Vector2 ratio);
    extern Vector2 get_pos_from_rel(Vector2 rel_pos);
    extern Vector2 get_pos_from_rel_center(Vector2 rel_pos, Vector2 size);
	extern Vector2 get_dock_size();
}
