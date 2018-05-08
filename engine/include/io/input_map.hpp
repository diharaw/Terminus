#pragma once

#include <core/terminus_macros.hpp>
#include <io/io_macros.hpp>
#include <io/input_device.hpp>
#include <stl/string_hash.hpp>
#include <stdint.h>
#include <array>

TE_BEGIN_TERMINUS_NAMESPACE

struct ActionMap
{
    uint32_t                                      count;
    std::array<uint64_t, TE_KEYBOARD_NUM_BUTTONS> keyboard;
    std::array<uint64_t, TE_MOUSE_NUM_BUTTONS>    mouse;
    std::array<uint64_t, TE_GAMEPAD_MAX>          gamepad;
};

struct StateMap
{
    uint32_t                                      count;
    std::array<uint64_t, TE_KEYBOARD_NUM_BUTTONS> keyboard;
    std::array<uint64_t, TE_MOUSE_NUM_BUTTONS>    mouse;
    std::array<uint64_t, TE_GAMEPAD_MAX>          gamepad;
};

struct AxisMap
{
    uint32_t                                      count;
    std::array<uint64_t, TE_KEYBOARD_NUM_BUTTONS> keyboard_pos;
    std::array<uint64_t, TE_KEYBOARD_NUM_BUTTONS> keyboard_neg;
    std::array<uint64_t, 3>                       mouse;
    std::array<uint64_t, TE_GAMEPAD_AXIS_MAX>     gamepad;
};

class InputMap
{
    friend class InputManager;
    
private:
    StringHash m_name;
    ActionMap  m_action_map;
    StateMap   m_state_map;
    AxisMap    m_axis_map;
    
public:
	InputMap();
    ~InputMap();
    StringHash name();
    uint32_t num_states();
    uint32_t num_axis();
    uint32_t num_actions();
    void set_action(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad);
    void set_state(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad);
    void set_axis(uint64_t hash, int32_t mouse, int32_t keyboard_pos, int32_t keyboard_neg, int32_t gamepad);
    void clear_action(StringHash hash);
    void clear_state(StringHash hash);
    void clear_axis(StringHash hash);
};

TE_END_TERMINUS_NAMESPACE
