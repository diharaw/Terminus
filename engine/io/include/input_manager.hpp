#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <io/include/input_device.hpp>
#include <io/include/input_map.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class InputManager
{
public:
    virtual uint32_t create_player()                                         = 0;
    virtual uint32_t num_players()                                           = 0;
    virtual void     destroy_player(uint32_t player)                         = 0;
    virtual void     set_input_map(InputMap* map, uint32_t player)           = 0;
    virtual uint32_t assign_gamepad(int32_t device_index, uint32_t player)   = 0;
    virtual void     resign_gamepad(uint32_t handle, uint32_t player)        = 0;
    virtual uint32_t assign_joystick(int32_t device_index, uint32_t player)  = 0;
    virtual void     resign_joystick(uint32_t handle, uint32_t player)       = 0;
    virtual uint32_t num_gamepad_device()                                    = 0;
    virtual uint32_t num_joystick_device()                                   = 0;
    virtual bool     keyboard_pressed(uint32_t code)                         = 0;
    virtual bool     mouse_pressed(uint32_t code)                            = 0;
    virtual double   mouse_axis(uint32_t code)                               = 0;
    virtual bool     gamepad_pressed(uint32_t code, uint32_t handle)         = 0;
    virtual double   gamepad_axis(uint32_t code, uint32_t handle)            = 0;
};

TE_END_TERMINUS_NAMESPACE
