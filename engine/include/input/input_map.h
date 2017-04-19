#pragma once

#include <container/hash_map.h>
#include <core/types.h>

#define MAX_ACTION 10
#define MAX_STATE 10
#define MAX_AXIS 10

namespace terminus
{
    struct ActionMap
    {
        HashMap<uint64_t, MAX_ACTION> _keyboard;
        HashMap<uint64_t, MAX_ACTION> _mouse;
        HashMap<uint64_t, MAX_ACTION> _gamepad;
    };
    
    struct StateMap
    {
        HashMap<uint64_t, MAX_STATE> _keyboard;
        HashMap<uint64_t, MAX_STATE> _mouse;
        HashMap<uint64_t, MAX_STATE> _gamepad;
    };
    
    struct AxisMap
    {
        HashMap<uint64_t, MAX_AXIS> _keyboard_pos;
        HashMap<uint64_t, MAX_AXIS> _keyboard_neg;
        HashMap<uint64_t, MAX_AXIS> _mouse;
        HashMap<uint64_t, MAX_AXIS> _gamepad;
    };
    
    struct InputMap
    {
        uint64_t  _name;
        ActionMap _action_map;
        StateMap  _state_map;
        AxisMap   _axis_map;
    };
}
