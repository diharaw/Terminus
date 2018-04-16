#pragma once

#include <io/include/input_map.hpp>
#include <io/include/input_device.hpp>
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

class InputMapImpl : public InputMap
{
    friend class InputManagerImpl;
    
private:
    StringHash m_name;
    ActionMap  m_action_map;
    StateMap   m_state_map;
    AxisMap    m_axis_map;
    
public:
	InputMapImpl();
    ~InputMapImpl();
    StringHash name() override;
    uint32_t num_states() override;
    uint32_t num_axis() override;
    uint32_t num_actions() override;
    void set_action(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad) override;
    void set_state(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad) override;
    void set_axis(uint64_t hash, int32_t mouse, int32_t keyboard_pos, int32_t keyboard_neg, int32_t gamepad) override;
    void clear_action(StringHash hash) override;
    void clear_state(StringHash hash) override;
    void clear_axis(StringHash hash) override;
};

TE_END_TERMINUS_NAMESPACE
