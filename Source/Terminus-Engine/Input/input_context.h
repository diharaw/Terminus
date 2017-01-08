#ifndef INPUTCONTEXT_H
#define INPUTCONTEXT_H

#include <string>
#include <unordered_map>
#include <types.h>

enum MouseAxis
{
    MOUSE_AXIS_X = 0,
    MOUSE_AXIS_Y = 1,
    MOUSE_WHEEL = 2
};

struct InputContext
{
    std::string m_ContextName;
    // Keyboard Mappings
    std::unordered_map<uint8, std::string> m_KeyboardStateMap;
    std::unordered_map<uint8, std::string> m_KeyboardActionMap;
    std::unordered_map<uint8, std::string> m_KeyboardAxisPositiveMap;
    std::unordered_map<uint8, std::string> m_KeyboardAxisNegativeMap;
    // Mouse Mappings
    std::unordered_map<uint8, std::string> m_MouseStateMap;
    std::unordered_map<uint8, std::string> m_MouseActionMap;
    std::unordered_map<uint8, std::string> m_MouseAxisMap;
    // Gamepad Mappings
    std::unordered_map<uint8, std::string> m_GamepadStateMap;
    std::unordered_map<uint8, std::string> m_GamepadActionMap;
    std::unordered_map<uint8, std::string> m_GamepadAxisMap;

    // Previous Value Maps
    std::unordered_map<std::string, float> m_PreviousValues;
};

#endif
