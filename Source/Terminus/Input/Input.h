#ifndef INPUT_H
#define INPUT_H

#include <string>

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYBOARD_BUTTONS 512

struct InputContext;

namespace Input
{
    struct GamepadDevice
    {
        
    };
    
    struct MouseDevice
    {
        bool button_states[MAX_MOUSE_BUTTONS];
        double x_position;
        double y_position;
    };
    
    struct KeyboardDevice
    {
        bool button_states[MAX_KEYBOARD_BUTTONS];
        double button_axis_states[MAX_KEYBOARD_BUTTONS];
    };
    
	extern void Initialize();
	extern void SetPlayerCount(int _Count);
	extern void LoadContext(std::string _Name, int _PlayerIndex = 0);
    extern InputContext* CreateContext(int _PlayerIndex = 0);
    extern void SetActiveContext(std::string _Name, int _PlayerIndex = 0);
    extern void ProcessKeyboardInput(int _Key, int _Action);
    extern void ProcessMouseButtonInput(int _Key, int _Action);
    extern void ProcessCursorInput(double _Xpos, int _Ypos);
    extern MouseDevice* GetMouseDevice();
    extern KeyboardDevice* GetKeyboardDevice();

    extern void GamepadCallback(int _Joy, int _Event);
}

#endif
