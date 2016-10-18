#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include <vector>
#include "InputContext.h"

struct PlayerContexts
{
    uint8 m_GamepadIndex;
    InputContext* m_ActiveContext;
    std::vector<InputContext> m_Contexts;
};

#endif
