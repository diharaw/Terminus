#pragma once

#include <container/hash_map.h>
#include <types.h>

#define MAX_ACTION 10
#define MAX_STATE 10
#define MAX_AXIS 10

namespace terminus
{
    enum MouseAxis
    {
        MOUSE_AXIS_X = 0,
        MOUSE_AXIS_Y = 1,
        MOUSE_WHEEL = 2
    };
    
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

//struct InputContext
//{
//    std::string m_ContextName;
//    // Keyboard Mappings
//    std::unordered_map<uint8, std::string> m_KeyboardStateMap;
//    std::unordered_map<uint8, std::string> m_KeyboardActionMap;
//    std::unordered_map<uint8, std::string> m_KeyboardAxisPositiveMap;
//    std::unordered_map<uint8, std::string> m_KeyboardAxisNegativeMap;
//    // Mouse Mappings
//    std::unordered_map<uint8, std::string> m_MouseStateMap;
//    std::unordered_map<uint8, std::string> m_MouseActionMap;
//    std::unordered_map<uint8, std::string> m_MouseAxisMap;
//    // Gamepad Mappings
//    std::unordered_map<uint8, std::string> m_GamepadStateMap;
//    std::unordered_map<uint8, std::string> m_GamepadActionMap;
//    std::unordered_map<uint8, std::string> m_GamepadAxisMap;
//
//    // Previous Value Maps
//    std::unordered_map<std::string, float> m_PreviousValues;
//};
