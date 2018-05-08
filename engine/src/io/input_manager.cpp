#include <io/input_manager.hpp>
#include <core/engine_core.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

InputManager::InputManager()
{

}

InputManager::~InputManager()
{
    
}

void InputManager::initialize()
{
    m_keyboard_states =  (uint8_t*)SDL_GetKeyboardState(NULL);
    m_default_player = UINT32_MAX;
    
    for (int i = 0; i < TE_KEYBOARD_NUM_BUTTONS; i++)
        m_button_axis_states[i] = 0.0;
    
    m_mouse_last_pos_x = mouse_axis(TE_MOUSE_AXIS_X);
    m_mouse_last_pos_y = mouse_axis(TE_MOUSE_AXIS_Y);
    m_mouse_last_pos_wheel = mouse_axis(TE_MOUSE_AXIS_WHEEL);
}

void InputManager::process_gamepad_added(int32_t device_index)
{
    
}

void InputManager::process_gamepad_removed(int32_t device_index)
{
    
}

void InputManager::process_joystick_added(int32_t device_index)
{
    
}

void InputManager::process_joystick_removed(int32_t device_index)
{
    
}

void InputManager::process_mouse_motion(int32_t xpos, int32_t ypos, int32_t xrel, int32_t yrel, bool relative)
{
	EventManager& event_manager = global::event_manager();

    if(m_default_player != UINT32_MAX)
    {
        auto& e = m_players.lookup(m_default_player);
        
        if(e.map)
        {
            if(e.map->m_axis_map.mouse[TE_MOUSE_AXIS_X - 9] != UINT64_MAX)
            {
                // Fire Mouse Axis Event
                Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                event->data.input_axis.hash = e.map->m_axis_map.mouse[TE_MOUSE_AXIS_X - 9];
                event->data.input_axis.value = xpos;
                
                if(relative)
                    event->data.input_axis.delta = xrel;
                else
                    event->data.input_axis.delta = xpos - m_mouse_last_pos_x;
                
                event_manager.queue_event(event);
            }
            if(e.map->m_axis_map.mouse[TE_MOUSE_AXIS_Y - 9] != UINT64_MAX)
            {
                // Fire Mouse Axis Event
                Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                event->data.input_axis.hash = e.map->m_axis_map.mouse[TE_MOUSE_AXIS_Y - 9];
                event->data.input_axis.value = ypos;
                
                if(relative)
                    event->data.input_axis.delta = yrel;
                else
                    event->data.input_axis.delta = ypos - m_mouse_last_pos_y;
                
                event_manager.queue_event(event);
            }
        }
    }
    
    m_mouse_last_pos_x = xpos;
    m_mouse_last_pos_y = ypos;
}

void InputManager::process_mouse_button(uint8_t key, uint32_t state)
{
	EventManager& event_manager = global::event_manager();

    if(m_default_player != UINT32_MAX)
    {
        auto& e = m_players.lookup(m_default_player);
        
        if(e.map)
        {
            // Check if Action
            if(state == TE_BUTTON_STATE_PRESSED)
            {
                if(e.map->m_action_map.mouse[key] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_ACTION_INPUT);
                    event->data.input_action.hash = e.map->m_action_map.mouse[key];
                    // Fire Event
                    event_manager.queue_event(event);
                    return;
                }
            }
            
            // Check if State
            if(e.map->m_state_map.mouse[key] != UINT64_MAX)
            {
                Event* event = event_manager.allocate_event(TE_EVENT_STATE_INPUT);
                event->data.input_state.hash = e.map->m_state_map.mouse[key];
                
                if(state == TE_BUTTON_STATE_PRESSED)
                {
                    // Fire Pressed Event
                    event->data.input_state.state = 1;
                    event_manager.queue_event(event);
                    return;
                }
                if(state == TE_BUTTON_STATE_RELEASED)
                {
                    // Fire Released Event
                    event->data.input_state.state = 0;
                    event_manager.queue_event(event);
                    return;
                }
            }
        }
    }
}

void InputManager::process_mouse_wheel(uint32_t value)
{
	EventManager& event_manager = global::event_manager();

    if(m_default_player != UINT32_MAX)
    {
        auto& e = m_players.lookup(m_default_player);
        
        if(e.map)
        {
            if(e.map->m_axis_map.mouse[TE_MOUSE_AXIS_WHEEL - 9] != UINT64_MAX)
            {
                // Fire Mouse Wheel Axis Event
                Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                event->data.input_axis.hash = e.map->m_axis_map.mouse[TE_MOUSE_AXIS_WHEEL - 9];
                event->data.input_axis.value = value;
                event->data.input_axis.delta = value - m_mouse_last_pos_wheel;
                event_manager.queue_event(event);
            }
        }
    }
    
    m_mouse_last_pos_wheel = value;
}

void InputManager::process_keyboard_button(uint32_t scan_code, uint32_t state)
{
	EventManager& event_manager = global::event_manager();

    if(m_default_player != UINT32_MAX)
    {
        auto& e = m_players.lookup(m_default_player);
        
        if(e.map)
        {
            // Check if Action
            if(state == TE_BUTTON_STATE_PRESSED)
            {
                if(e.map->m_action_map.keyboard[scan_code] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_ACTION_INPUT);
                    event->data.input_action.hash = e.map->m_action_map.keyboard[scan_code];
                    // Fire Event
                    event_manager.queue_event(event);
                    return;
                }
            }
            
            // Check if State
            if(e.map->m_state_map.keyboard[scan_code] != UINT64_MAX)
            {
                Event* event = event_manager.allocate_event(TE_EVENT_STATE_INPUT);
                event->data.input_state.hash = e.map->m_state_map.keyboard[scan_code];
                
                if(state == TE_BUTTON_STATE_PRESSED)
                {
                    // Fire Pressed Event
                    event->data.input_state.state = 1;
                    event_manager.queue_event(event);
                    return;
                }
                if(state == TE_BUTTON_STATE_RELEASED)
                {
                    // Fire Released Event
                    event->data.input_state.state = 0;
                    event_manager.queue_event(event);
                    return;
                }
            }
            
            // Check if Axis Press
            if(state == TE_BUTTON_STATE_PRESSED)
            {
                // Check if Positive Axis
                if(e.map->m_axis_map.keyboard_pos[scan_code] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                    event->data.input_axis.hash = e.map->m_axis_map.keyboard_pos[scan_code];
                    
                    double last_value = m_button_axis_states[scan_code];
                    
                    // Fire Axis Positive Event
                    event->data.input_axis.value = 1.0;
                    event->data.input_axis.delta = 1.0 - last_value;
                    event_manager.queue_event(event);
                    
                    m_button_axis_states[scan_code] = 1.0;
                    return;
                }
                
                // Check if Negative Axis
                if(e.map->m_axis_map.keyboard_neg[scan_code] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                    event->data.input_axis.hash = e.map->m_axis_map.keyboard_neg[scan_code];
                    
                    double last_value = m_button_axis_states[scan_code];
                    
                    // Fire Axis Negative Event
                    event->data.input_axis.value = -1.0;
                    event->data.input_axis.delta = -1.0 - last_value;
                    event_manager.queue_event(event);
                    
                    m_button_axis_states[scan_code] = -1.0;
                    return;
                }
            }
            
            // Check if Axis Release
            if(state == TE_BUTTON_STATE_RELEASED)
            {
                if(e.map->m_axis_map.keyboard_pos[scan_code] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                    event->data.input_axis.hash = e.map->m_axis_map.keyboard_pos[scan_code];
                    
                    double last_value = m_button_axis_states[scan_code];
                    
                    // Fire Axis Positive Event
                    event->data.input_axis.value = 0.0;
                    event->data.input_axis.delta = 0.0 - last_value;
                    event_manager.queue_event(event);
                    
                    m_button_axis_states[scan_code] = 0.0;
                    return;
                }
                if(e.map->m_axis_map.keyboard_neg[scan_code] != UINT64_MAX)
                {
                    Event* event = event_manager.allocate_event(TE_EVENT_AXIS_INPUT);
                    event->data.input_axis.hash = e.map->m_axis_map.keyboard_neg[scan_code];
                    
                    double last_value = m_button_axis_states[scan_code];
                    
                    // Fire Axis Negative Event
                    event->data.input_axis.value = 0.0;
                    event->data.input_axis.delta = 0.0 - last_value;
                    event_manager.queue_event(event);
                    
                    m_button_axis_states[scan_code] = 0.0;
                    return;
                }
            }
        }
    }
}

void InputManager::process_gamepad_axis(uint8_t axis, int16_t value, int32_t device_id)
{
    PlayerEntry* players = m_players.array();
    
    for(uint32_t i = 0; i < m_players.size(); i++)
    {
        
    }
}

void InputManager::process_gamepad_button(uint8_t button, uint8_t state, int32_t device_id)
{
    PlayerEntry* players = m_players.array();
    
    for(uint32_t i = 0; i < m_players.size(); i++)
    {
        
    }
}

void InputManager::process_joystick_axis(uint8_t axis, int32_t value, int32_t device_id)
{
    PlayerEntry* players = m_players.array();
    
    for(uint32_t i = 0; i < m_players.size(); i++)
    {
        
    }
}

void InputManager::process_joystick_button(uint8_t button, uint8_t state, int32_t device_id)
{
    PlayerEntry* players = m_players.array();
    
    for(uint32_t i = 0; i < m_players.size(); i++)
    {
        
    }
}

void InputManager::process_joystick_hat(uint8_t hat, uint8_t value, int32_t device_id)
{
    PlayerEntry* players = m_players.array();
    
    for(uint32_t i = 0; i < m_players.size(); i++)
    {
        
    }
}

uint32_t InputManager::create_player()
{
    uint32_t handle = m_players.add();
    auto& e = m_players.lookup(handle);
    
    e.gamepad_handle = UINT32_MAX;
    e.map = nullptr;
    
    if(m_default_player == UINT32_MAX)
        m_default_player = handle;
    
    return handle;
}

uint32_t InputManager::num_players()
{
    return m_players.size();
}

void InputManager::destroy_player(uint32_t player)
{
    if(m_players.has(player))
    {
        if(player == m_default_player)
            m_default_player = UINT32_MAX;
        
        auto& e = m_players.lookup(player);
        resign_gamepad(e.gamepad_handle, player);
//        resign_joystick(e.gamepad_handle, player); TODO: add joystick handles
        m_players.remove(player);
    }
}

void InputManager::set_input_map(InputMap* map, uint32_t player)
{
    if(m_players.has(player))
    {
        auto& e = m_players.lookup(player);
        e.map = map;
    }
}

uint32_t InputManager::assign_gamepad(int32_t device_index, uint32_t player)
{
    if(m_players.has(player))
    {
        PlayerEntry& e = m_players.lookup(player);
        
        uint32_t handle = m_gamepads.add();
        auto& ge = m_gamepads.lookup(handle);
        
        e.gamepad_handle = handle;
        
        ge.controller = SDL_GameControllerOpen(device_index);
        ge.joystick = SDL_GameControllerGetJoystick(ge.controller);
        ge.id = SDL_JoystickInstanceID(ge.joystick);
        
        return handle;
    }
    else
        return UINT32_MAX;
}

void InputManager::resign_gamepad(uint32_t handle, uint32_t player)
{
    if(m_players.has(player))
    {
        if(m_gamepads.has(handle))
        {
            auto& e = m_gamepads.lookup(handle);
            SDL_GameControllerClose(e.controller);
            
            e.controller = nullptr;
            e.joystick = nullptr;
            e.id = UINT32_MAX;
            
            m_gamepads.remove(handle);
        }
    }
}

uint32_t InputManager::assign_joystick(int32_t device_index, uint32_t player)
{
    return 0; // TODO: implement this!
}

void InputManager::resign_joystick(uint32_t handle, uint32_t player)
{
    // TODO: implement this!
}

uint32_t InputManager::num_gamepad_device()
{
    uint32_t count = 0;
    
    for(int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
            count++;
    }
    
    return count;
}

uint32_t InputManager::num_joystick_device()
{
    uint32_t count = 0;
    
    for(int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (!SDL_IsGameController(i))
            count++;
    }
    
    return count;
}

bool InputManager::keyboard_pressed(uint32_t code)
{
    return m_keyboard_states[code];
}

bool InputManager::mouse_pressed(uint32_t code)
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(code);
}

double InputManager::mouse_axis(uint32_t code)
{
    int32_t pos[2];
    SDL_GetMouseState(&pos[0], &pos[1]);
    return pos[code - 9];
}

bool InputManager::gamepad_pressed(uint32_t code, uint32_t handle)
{
    if(m_gamepads.has(handle))
    {
        auto& e = m_gamepads.lookup(handle);
        return static_cast<bool>(SDL_GameControllerGetButton(e.controller, static_cast<SDL_GameControllerButton>(code)));
    }
    else
        return false;
}

double InputManager::gamepad_axis(uint32_t code, uint32_t handle)
{
    if(m_gamepads.has(handle))
    {
        auto& e = m_gamepads.lookup(handle);
        return static_cast<bool>(SDL_GameControllerGetAxis(e.controller, static_cast<SDL_GameControllerAxis>(code)));
    }
    else
        return 0.0;
}

TE_END_TERMINUS_NAMESPACE
