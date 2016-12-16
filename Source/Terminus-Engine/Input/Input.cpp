#include "Input.h"
#include "../Core/Event/Event.h"
#include "../Core/Event/EventHandler.h"
#include "PlayerContext.h"
#include <vector>

#define MAX_PLAYERS 8

namespace Input
{
    int m_PlayerCount;
    std::vector<PlayerContexts> m_PlayerContextList;
    KeyboardDevice m_keyboard;
    MouseDevice    m_mouse;

    void FindPlayerDevices()
    {
//         for (int i = m_PlayerContextList.size(); i < m_PlayerCount; i++)
//        {
//            bool present = (bool)glfwJoystickPresent(GLFW_JOYSTICK_1 + i);
//
//            if(present)
//            {
//                if(i > m_PlayerContextList.size() - 1)
//                {
//                    PlayerContexts newPlayer = PlayerContexts();
//                    m_PlayerContextList.push_back(newPlayer);
//                }
//
//                m_PlayerContextList[i].m_GamepadIndex = GLFW_JOYSTICK_1 + i;
//            }
//        }
    }

    void Initialize()
    {
        PlayerContexts initialPlayer = PlayerContexts();
        m_PlayerContextList.push_back(initialPlayer);
        m_PlayerCount = 1;

        for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
        {
            m_mouse.button_states[i] = false;
        }
        
        for (int i = 0; i < MAX_KEYBOARD_BUTTONS; i++)
        {
            m_keyboard.button_states[i] = false;
            m_keyboard.button_axis_states[i] = 0.0;
        }
        
        FindPlayerDevices();
    }
    
    MouseDevice* GetMouseDevice()
    {
        return &m_mouse;
    }
    
    KeyboardDevice* GetKeyboardDevice()
    {
        return &m_keyboard;
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
    
    InputContext* CreateContext(int _PlayerIndex)
    {
        PlayerContexts* player_context = &m_PlayerContextList[_PlayerIndex];
        
        InputContext input_context;
        int size = player_context->m_Contexts.size();
        player_context->m_Contexts.push_back(input_context);
        
        return &player_context->m_Contexts[size];
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

    void ProcessKeyboardInput(Sint32 _Key, Uint32 _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            // Check if State
            if(context->m_KeyboardStateMap.find(_Key) != context->m_KeyboardStateMap.end() && _Action == SDL_KEYDOWN)
            {
                std::string state = context->m_KeyboardStateMap[_Key];
                
                // Fire Event
                InputStateEvent* event = new InputStateEvent(state);
                Terminus::EventHandler::QueueEvent(event);
                
                m_keyboard.button_states[_Key] = _Action;
                
                return;
            }
            
            // Check if Action
            if(context->m_KeyboardActionMap.find(_Key) != context->m_KeyboardActionMap.end())
            {
                std::string action = context->m_KeyboardActionMap[_Key];
                
                if(_Action == SDL_KEYDOWN)
                {
                    // Fire Pressed Event
                    InputActionEvent* event = new InputActionEvent(action, 1);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                if(_Action == SDL_KEYUP)
                {
                    // Fire Released Event
                    InputActionEvent* event = new InputActionEvent(action, 0);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                
                m_keyboard.button_states[_Key] = _Action;
            }
            
            // Check if Axis Press
            if(_Action == SDL_KEYDOWN)
            {
                // Check if Positive Axis
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    
                    double last_value = m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Positive Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 1.0, 1.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 1.0;
                    
                    return;
                }
                
                // Check if Negative Axis
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Negative Event
                    InputAxisEvent* event = new InputAxisEvent(axis, -1.0, -1.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = -1.0;
                    
                    return;
                }
            }
            
            // Check if Axis Release
            if(_Action == SDL_KEYUP)
            {
                if(context->m_KeyboardAxisPositiveMap.find(_Key) != context->m_KeyboardAxisPositiveMap.end())
                {
                    std::string axis = context->m_KeyboardAxisPositiveMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Positive Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 0.0, 0.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 0.0;
                    
                    return;
                }
                if(context->m_KeyboardAxisNegativeMap.find(_Key) != context->m_KeyboardAxisNegativeMap.end())
                {
                    std::string axis = context->m_KeyboardAxisNegativeMap[_Key];
                    
                    double last_value = (double)m_keyboard.button_axis_states[_Key];
                    
                    // Fire Axis Negative Event
                    InputAxisEvent* event = new InputAxisEvent(axis, 0.0, 0.0 - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    m_keyboard.button_axis_states[_Key] = 0.0;
                    
                    return;
                }
            }
        }
    }
    
    void ProcessMouseButtonInput(Uint8 _Key, Uint32 _Action)
    {
        if(m_PlayerContextList.size() != 0)
        {
            // For each Player
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            // Check if State
            if(context->m_MouseStateMap.find((uint8)_Key) != context->m_MouseStateMap.end() && _Action == SDL_KEYDOWN)
            {
                std::string state = context->m_MouseStateMap[_Key];
                // Fire Event
                InputStateEvent* event = new InputStateEvent(state);
                Terminus::EventHandler::QueueEvent(event);
                
                return;
            }
            
            // Check if Action
            if(context->m_MouseActionMap.find((uint8)_Key) != context->m_MouseActionMap.end())
            {
                std::string action = context->m_MouseActionMap[_Key];
                
                if(_Action == SDL_KEYDOWN)
                {
                    // Fire Pressed Event
                    InputActionEvent* event = new InputActionEvent(action, 1);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
                if(_Action == SDL_KEYUP)
                {
                    // Fire Released Event
                    InputActionEvent* event = new InputActionEvent(action, 0);
                    Terminus::EventHandler::QueueEvent(event);
                    
                    return;
                }
            }
        }
        
        m_mouse.button_states[_Key] = _Action;
    }
    
    void ProcessCursorInput(double _Xpos, int _Ypos)
    {
        if(m_PlayerContextList.size() != 0)
        {
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            for (auto it : context->m_MouseAxisMap)
            {
                if(it.first == MOUSE_AXIS_X)
                {
                    double last_position = m_mouse.x_position;
                    // Fire Mouse Axis Event
                    InputAxisEvent* event = new InputAxisEvent(it.second, _Xpos, _Xpos - last_position);
                    Terminus::EventHandler::QueueEvent(event);
                }
                if(it.first == MOUSE_AXIS_Y)
                {
                    double last_position = m_mouse.y_position;
                    // Fire Mouse Axis Event
                    InputAxisEvent* event = new InputAxisEvent(it.second, _Ypos, _Ypos - last_position);
                    Terminus::EventHandler::QueueEvent(event);
                }
            }
        }
        
        m_mouse.x_position = _Xpos;
        m_mouse.y_position = _Ypos;
    }
    
    void ProcessMouseWheelInput(Uint32 value)
    {
        if(m_PlayerContextList.size() != 0)
        {
            InputContext* context = m_PlayerContextList[0].m_ActiveContext;
            
            if(!context)
                return;
            
            for (auto it : context->m_MouseAxisMap)
            {
                if(it.first == MOUSE_WHEEL)
                {
                    Uint32 last_value = m_mouse.wheel;
                    InputAxisEvent* event = new InputAxisEvent(it.second, value, value - last_value);
                    Terminus::EventHandler::QueueEvent(event);
                }
            }
        }
    }
    
    void ProcessWindowEvents(SDL_Event& event)
    {
        switch (event.type) {
            case SDL_MOUSEWHEEL:
                ProcessMouseWheelInput(event.wheel.y);
                break;
                
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
                ProcessMouseButtonInput(event.button.button, event.type);
                break;
                
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                ProcessKeyboardInput(event.key.keysym.sym, event.type);
                break;
                
            default:
                break;
        }
    }

}
