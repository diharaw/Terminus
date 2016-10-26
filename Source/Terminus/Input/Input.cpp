#include "Input.h"
#include "PlayerContext.h"
#include <vector>
#include <GLFW/glfw3.h>

#define MAX_PLAYERS 8

namespace Input
{
    int m_PlayerCount;
    std::vector<PlayerContexts> m_PlayerContextList;

    void FindPlayerDevices()
    {
         for (int i = m_PlayerContextList.size(); i < m_PlayerCount; i++)
        {
            bool present = (bool)glfwJoystickPresent(GLFW_JOYSTICK_1 + i);

            if(present)
            {
                if(i > m_PlayerContextList.size() - 1)
                {
                    PlayerContexts newPlayer = PlayerContexts();
                    m_PlayerContextList.push_back(newPlayer);
                }

                m_PlayerContextList[i].m_GamepadIndex = GLFW_JOYSTICK_1 + i;
            }
        }
    }

    void Initialize()
    {
        PlayerContexts initialPlayer = PlayerContexts();
        m_PlayerContextList.push_back(initialPlayer);
        m_PlayerCount = 1;

        FindPlayerDevices();
    }

    void SetPlayerCount(int _Count)
    {
        m_PlayerCount = _Count;
        FindPlayerDevices();
    }

    void LoadContext(std::string _Name, int _PlayerIndex)
    {
        if(_PlayerIndex > m_PlayerContextList.size() - 1)
            return;
        else
        {
            PlayerContexts* cxt = &m_PlayerContextList[_PlayerIndex];
            // Load context
            
            // Add to Player Context
        }
    }

    void SetActiveContext(std::string _Name, int _PlayerIndex)
    {
        if(_PlayerIndex > m_PlayerContextList.size() - 1)
            return;
        else
        {
            PlayerContexts* cxt = &m_PlayerContextList[_PlayerIndex];

            for (int i = 0; i < cxt->m_Contexts.size(); i++)
            {
                if(cxt->m_Contexts[i].m_ContextName == _Name)
                {
                    cxt->m_ActiveContext = &cxt->m_Contexts[i];
                    break;
                }
            }
        }
    }

    void ProcessKeyboardInput(int _Key, int _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            // Check if State
            if(context->m_KeyboardStateMap.find(_Key) != context->m_KeyboardStateMap.end() && _Action == GLFW_PRESS)
            {
                std::string state = context->m_KeyboardStateMap[_Key];
                // Fire Event
                return;
            }
            
            // Check if State
            if(context->m_KeyboardActionMap.find(_Key) != context->m_KeyboardActionMap.end())
            {
                std::string action = context->m_KeyboardStateMap[_Key];
                
                if(_Action == GLFW_PRESS)
                {
                    // Fire Pressed Event
                    return;
                }
                if(_Action == GLFW_RELEASE)
                {
                    // Fire Released Event
                    return;
                }
            }
            
            // Check if Positive Axis
            if(_Action == GLFW_PRESS)
            {
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    // Fire Axis Positive Event
                    return;
                }
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    // Fire Axis Negative Event
                    return;
                }
            }
            
            // Check if Negative Axis
            if(_Action == GLFW_RELEASE)
            {
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    // Fire Axis Positive Event
                    return;
                }
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    // Fire Axis Negative Event
                    return;
                }
            }
        }
    }
    
    void ProcessMouseButtonInput(int _Key, int _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            // Check if State
            if(context->m_MouseStateMap.find((uint8)_Key) != context->m_MouseStateMap.end() && _Action == GLFW_PRESS)
            {
                std::string state = context->m_MouseStateMap[_Key];
                // Fire Event
                return;
            }
            
            // Check if Action
            if(context->m_MouseActionMap.find((uint8)_Key) != context->m_MouseActionMap.end())
            {
                std::string action = context->m_MouseActionMap[_Key];
                
                if(_Action == GLFW_PRESS)
                {
                    // Fire Pressed Event
                    return;
                }
                if(_Action == GLFW_RELEASE)
                {
                    // Fire Released Event
                    return;
                }
            }
        }
    }
    
    void ProcessCursorInput(double _Xpos, int _Ypos)
    {
        if(m_PlayerContextList.size() != 0)
        {
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            for (auto it : context->m_MouseAxisMap)
            {
                if(it.first == MOUSE_AXIS_X)
                {
                    // Fire Mouse Axis Event
                }
                if(it.first == MOUSE_AXIS_Y)
                {
                    // Fire Mouse Axis Event
                }
            }
        }
    }

    void GamepadCallback(int _Joy, int _Event)
    {
        if(_Event == GLFW_CONNECTED)
        {

        }
        else if(_Event == GLFW_DISCONNECTED)
        {

        }
    }
}
