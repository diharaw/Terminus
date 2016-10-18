#ifndef INPUT_H
#define INPUT_H

#include <string>

namespace Input
{
	extern void Initialize();
	extern void SetPlayerCount(int _Count);
	extern void LoadContext(std::string _Name, int _PlayerIndex = 0);
    extern void SetActiveContext(std::string _Name, int _PlayerIndex = 0);
    extern void ProcessKeyboardInput(int _Key, int _Action);
    extern void ProcessMouseButtonInput(int _Key, int _Action);
    extern void ProcessCursorInput(double _Xpos, int _Ypos);

    extern void GamepadCallback(int _Joy, int _Event);
}

#endif
