#include <Input/input_handler.h>
#include <Input/player_context.h>
#include <Core/Event/event.h>
#include <Core/Event/event_handler.h>
#include <Utility/murmur_hash.h>
#include <Resource/text_loader.h>
#include <IO/filesystem.h>

#include <vector>

#define MAX_PLAYERS 8
#define MAX_KEYBOARD_INPUTS 100
#define MAX_MOUSE_INPUTS 10
#define MAX_GAMEPAD_INPUTS 20

namespace terminus
{
    namespace input_handler
    {
        int                                      _player_count;
        HandleArray<PlayerContext, MAX_PLAYERS>  _player_contexts;
        KeyboardDevice                           _keyboard_device;
        MouseDevice                              _mouse_device;
        GamepadDevice                            _gamepad_device; // temp
        PlayerContextHandle                      _default_player_context;
        HashMap<Sint32, MAX_KEYBOARD_INPUTS>     _keyboard_code_map;
        HashMap<Uint8, MAX_MOUSE_INPUTS>        _mouse_code_map;
        HashMap<Uint8, MAX_GAMEPAD_INPUTS>      _gamepad_code_map;
        
        void initialize()
        {
            _default_player_context = _player_contexts.create();
            _player_count = 1;
            
            for (int i = 0; i < MAX_MOUSE_BUTTONS; i++)
            {
                _mouse_device.button_states[i] = false;
            }
            
            for (int i = 0; i < MAX_KEYBOARD_BUTTONS; i++)
            {
                _keyboard_device.button_states[i] = false;
                _keyboard_device.button_axis_states[i] = 0.0;
            }
            
            // setup keyboard codes
            _keyboard_code_map.set(HASH("A"), SDLK_a);
            _keyboard_code_map.set(HASH("B"), SDLK_b);
            _keyboard_code_map.set(HASH("C"), SDLK_c);
            _keyboard_code_map.set(HASH("D"), SDLK_d);
            _keyboard_code_map.set(HASH("E"), SDLK_e);
            _keyboard_code_map.set(HASH("F"), SDLK_f);
            _keyboard_code_map.set(HASH("G"), SDLK_g);
            _keyboard_code_map.set(HASH("H"), SDLK_h);
            _keyboard_code_map.set(HASH("I"), SDLK_i);
            _keyboard_code_map.set(HASH("J"), SDLK_j);
            _keyboard_code_map.set(HASH("K"), SDLK_k);
            _keyboard_code_map.set(HASH("L"), SDLK_l);
            _keyboard_code_map.set(HASH("M"), SDLK_m);
            _keyboard_code_map.set(HASH("N"), SDLK_n);
            _keyboard_code_map.set(HASH("O"), SDLK_o);
            _keyboard_code_map.set(HASH("P"), SDLK_p);
            _keyboard_code_map.set(HASH("Q"), SDLK_q);
            _keyboard_code_map.set(HASH("R"), SDLK_r);
            _keyboard_code_map.set(HASH("S"), SDLK_s);
            _keyboard_code_map.set(HASH("T"), SDLK_t);
            _keyboard_code_map.set(HASH("U"), SDLK_u);
            _keyboard_code_map.set(HASH("V"), SDLK_v);
            _keyboard_code_map.set(HASH("W"), SDLK_w);
            _keyboard_code_map.set(HASH("X"), SDLK_x);
            _keyboard_code_map.set(HASH("Y"), SDLK_y);
            _keyboard_code_map.set(HASH("Z"), SDLK_z);
            _keyboard_code_map.set(HASH("1"), SDLK_1);
            _keyboard_code_map.set(HASH("2"), SDLK_2);
            _keyboard_code_map.set(HASH("3"), SDLK_3);
            _keyboard_code_map.set(HASH("4"), SDLK_4);
            _keyboard_code_map.set(HASH("5"), SDLK_5);
            _keyboard_code_map.set(HASH("6"), SDLK_6);
            _keyboard_code_map.set(HASH("7"), SDLK_7);
            _keyboard_code_map.set(HASH("8"), SDLK_8);
            _keyboard_code_map.set(HASH("9"), SDLK_9);
            _keyboard_code_map.set(HASH("0"), SDLK_0);
            _keyboard_code_map.set(HASH("RETURN"), SDLK_RETURN);
            _keyboard_code_map.set(HASH("ESCAPE"), SDLK_ESCAPE);
            _keyboard_code_map.set(HASH("BACKSPACE"), SDLK_BACKSPACE);
            _keyboard_code_map.set(HASH("TAB"), SDLK_TAB);
            _keyboard_code_map.set(HASH("SPACE"), SDLK_SPACE);
            _keyboard_code_map.set(HASH("LSHIFT"), SDLK_LSHIFT);
            _keyboard_code_map.set(HASH("RSHIFR"), SDLK_RSHIFT);
            _keyboard_code_map.set(HASH("LCTRL"), SDLK_LCTRL);
            _keyboard_code_map.set(HASH("RCTRL"), SDLK_RCTRL);
            _keyboard_code_map.set(HASH("INSERT"), SDLK_INSERT);
            _keyboard_code_map.set(HASH("HOME"), SDLK_HOME);
            _keyboard_code_map.set(HASH("PAGEUP"), SDLK_PAGEUP);
            _keyboard_code_map.set(HASH("DELETE"), SDLK_DELETE);
            _keyboard_code_map.set(HASH("END"), SDLK_END);
            _keyboard_code_map.set(HASH("PAGEDOWN"), SDLK_PAGEUP);
            _keyboard_code_map.set(HASH("RIGHT"), SDLK_RIGHT);
            _keyboard_code_map.set(HASH("LEFT"), SDLK_LEFT);
            _keyboard_code_map.set(HASH("UP"), SDLK_UP);
            _keyboard_code_map.set(HASH("DOWN"), SDLK_DOWN);
            _keyboard_code_map.set(HASH("KP_NUMLOCK"), SDLK_NUMLOCKCLEAR);
            _keyboard_code_map.set(HASH("KP_DIVIDE"), SDLK_KP_DIVIDE);
            _keyboard_code_map.set(HASH("KP_MULTIPLY"), SDLK_KP_MULTIPLY);
            _keyboard_code_map.set(HASH("KP_MINUS"), SDLK_KP_MINUS);
            _keyboard_code_map.set(HASH("KP_PLUS"), SDLK_KP_PLUS);
            _keyboard_code_map.set(HASH("KP_ENTER"), SDLK_KP_ENTER);
            _keyboard_code_map.set(HASH("KP_1"), SDLK_KP_1);
            _keyboard_code_map.set(HASH("KP_2"), SDLK_KP_2);
            _keyboard_code_map.set(HASH("KP_3"), SDLK_KP_3);
            _keyboard_code_map.set(HASH("KP_4"), SDLK_KP_4);
            _keyboard_code_map.set(HASH("KP_5"), SDLK_KP_5);
            _keyboard_code_map.set(HASH("KP_6"), SDLK_KP_6);
            _keyboard_code_map.set(HASH("KP_7"), SDLK_KP_7);
            _keyboard_code_map.set(HASH("KP_8"), SDLK_KP_8);
            _keyboard_code_map.set(HASH("KP_9"), SDLK_KP_9);
            _keyboard_code_map.set(HASH("KP_0"), SDLK_KP_0);
            _keyboard_code_map.set(HASH("KP_PERIOD"), SDLK_KP_PERIOD);
            
            // mouse codes
            _mouse_code_map.set(HASH("MOUSE_LEFT"), SDL_BUTTON_LEFT);
            _mouse_code_map.set(HASH("MOUSE_RIGHT"), SDL_BUTTON_RIGHT);
            _mouse_code_map.set(HASH("MOUSE_MIDDLE"), SDL_BUTTON_MIDDLE);
            _mouse_code_map.set(HASH("MOUSE_AXIS_X"), MOUSE_AXIS_X);
            _mouse_code_map.set(HASH("MOUSE_AXIS_Y"), MOUSE_AXIS_Y);
            _mouse_code_map.set(HASH("MOUSE_WHEEL"), MOUSE_WHEEL);
            
            // gamepad codes
            _gamepad_code_map.set(HASH("GAMEPAD_X"), SDL_CONTROLLER_BUTTON_X);
            _gamepad_code_map.set(HASH("GAMEPAD_Y"), SDL_CONTROLLER_BUTTON_Y);
            _gamepad_code_map.set(HASH("GAMEPAD_A"), SDL_CONTROLLER_BUTTON_A);
            _gamepad_code_map.set(HASH("GAMEPAD_B"), SDL_CONTROLLER_BUTTON_B);
            _gamepad_code_map.set(HASH("GAMEPAD_LEFT_X"), SDL_CONTROLLER_AXIS_LEFTX);
            _gamepad_code_map.set(HASH("GAMEPAD_LEFT_Y"), SDL_CONTROLLER_AXIS_LEFTY);
            _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_X"), SDL_CONTROLLER_AXIS_RIGHTX);
            _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_Y"), SDL_CONTROLLER_AXIS_RIGHTY);
            _gamepad_code_map.set(HASH("GAMEPAD_DPAD_LEFT"), SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            _gamepad_code_map.set(HASH("GAMEPAD_DPAD_RIGHT"), SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
            _gamepad_code_map.set(HASH("GAMEPAD_DPAD_UP"), SDL_CONTROLLER_BUTTON_DPAD_UP);
            _gamepad_code_map.set(HASH("GAMEPAD_DPAD_DOWN"), SDL_CONTROLLER_BUTTON_DPAD_DOWN);
            _gamepad_code_map.set(HASH("GAMEPAD_LEFT_STICK"), SDL_CONTROLLER_BUTTON_LEFTSTICK);
            _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_STICK"), SDL_CONTROLLER_BUTTON_RIGHTSTICK);
            _gamepad_code_map.set(HASH("GAMEPAD_LEFT_SHOULDER"), SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
            _gamepad_code_map.set(HASH("GAMEPAD_RIGHT_SHOULDER"), SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
            _gamepad_code_map.set(HASH("GAMEPAD_TRIGGER_LEFT"), SDL_CONTROLLER_AXIS_TRIGGERLEFT);
            _gamepad_code_map.set(HASH("GAMEPAD_TRIGGER_RIGHT"), SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            
            filesystem::add_directory("config/input_map");
        }
        
        MouseDevice& get_mouse_device()
        {
            return _mouse_device;
        }
        
        KeyboardDevice& get_keyboard_device()
        {
            return _keyboard_device;
        }
        
        GamepadDevice& get_gamepad_device(PlayerContextHandle player)
        {
            return _gamepad_device;
        }
        
        InputMapHandle load_input_map(PlayerContextHandle player, String name)
        {
            if(_player_contexts.has(player))
            {
                PlayerContext* cxt = _player_contexts.get_ptr(player);
                // Load context
                
                asset_common::TextLoadData* load_data = text_loader::load(String(name.c_str()));
                InputMapHandle input_map_handle = cxt->_input_maps.create();
                InputMap* input_map = cxt->_input_maps.get_ptr(input_map_handle);
                
                if(load_data->buffer)
                {
                    JsonDocument doc;
                    doc.Parse(load_data->buffer);
                    
                    // parse actions
                    assert(doc.HasMember("action"));
                    {
                        JsonValue& actions = doc["action"];
                        
                        for (rapidjson::SizeType i = 0; i < actions.Size(); i++)
                        {
                            assert(actions[i].HasMember("name"));
                            
                            uint64_t name_hash = HASH(actions[i]["name"].GetString());
                            
                            if(actions[i].HasMember("keyboard"))
                            {
                                if(!actions[i]["keyboard"].IsNull())
                                {
                                    uint64_t kb_hash = HASH(actions[i]["keyboard"].GetString());
                                    assert(_keyboard_code_map.has(kb_hash));
                                    Sint32 value;
                                    _keyboard_code_map.get(kb_hash, value);
                                    
                                    input_map->_action_map._keyboard.set(value, name_hash);
                                }
                            }
                            
                            if(actions[i].HasMember("mouse"))
                            {
                                if(!actions[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(actions[i]["mouse"].GetString());
                                    assert(_mouse_code_map.has(mouse_hash));
                                    Uint8 value;
                                    _mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_action_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(actions[i].HasMember("gamepad"))
                            {
                                if(!actions[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(actions[i]["gamepad"].GetString());
                                    assert(_gamepad_code_map.has(gp_hash));
                                    Uint8 value;
                                    _gamepad_code_map.get(gp_hash, value);
                                    
                                    input_map->_action_map._gamepad.set(value, name_hash);
                                }
                            }
                        }
                    }
                    
                    // parse states
                    assert(doc.HasMember("state"));
                    {
                        JsonValue& states = doc["state"];
                        
                        for (rapidjson::SizeType i = 0; i < states.Size(); i++)
                        {
                            assert(states[i].HasMember("name"));
                            
                            uint64_t name_hash = HASH(states[i]["name"].GetString());
                            
                            if(states[i].HasMember("keyboard"))
                            {
                                if(!states[i]["keyboard"].IsNull())
                                {
                                    uint64_t kb_hash = HASH(states[i]["keyboard"].GetString());
                                    assert(_keyboard_code_map.has(kb_hash));
                                    Sint32 value;
                                    _keyboard_code_map.get(kb_hash, value);
                                    
                                    input_map->_state_map._keyboard.set(value, name_hash);
                                }
                            }
                            
                            if(states[i].HasMember("mouse"))
                            {
                                if(!states[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(states[i]["mouse"].GetString());
                                    assert(_mouse_code_map.has(mouse_hash));
                                    Uint8 value;
                                    _mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_state_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(states[i].HasMember("gamepad"))
                            {
                                if(!states[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(states[i]["gamepad"].GetString());
                                    assert(_gamepad_code_map.has(gp_hash));
                                    Uint8 value;
                                    _gamepad_code_map.get(gp_hash, value);
                                    
                                    input_map->_state_map._gamepad.set(value, name_hash);
                                }
                            }
                        }
                    }
                    
                    // parse axis
                    assert(doc.HasMember("axis"));
                    {
                        JsonValue& axis = doc["axis"];
                        
                        for (rapidjson::SizeType i = 0; i < axis.Size(); i++)
                        {
                            assert(axis[i].HasMember("name"));
                            
                            uint64_t name_hash = HASH(axis[i]["name"].GetString());
                            
                            if(axis[i].HasMember("keyboard_pos") && axis[i].HasMember("keyboard_neg"))
                            {
                                if(!axis[i]["keyboard_pos"].IsNull() && !axis[i]["keyboard_neg"].IsNull())
                                {
                                    // positive value
                                    {
                                        uint64_t kb_hash = HASH(axis[i]["keyboard_pos"].GetString());
                                        assert(_keyboard_code_map.has(kb_hash));
                                        Sint32 value;
                                        _keyboard_code_map.get(kb_hash, value);
                                        
                                        input_map->_axis_map._keyboard_pos.set(value, name_hash);
                                    }
                                    
                                    // negative value
                                    {
                                        uint64_t kb_hash = HASH(axis[i]["keyboard_neg"].GetString());
                                        assert(_keyboard_code_map.has(kb_hash));
                                        Sint32 value;
                                        _keyboard_code_map.get(kb_hash, value);
                                        
                                        input_map->_axis_map._keyboard_neg.set(value, name_hash);
                                    }
                                }
                            }
                            
                            if(axis[i].HasMember("mouse"))
                            {
                                if(!axis[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(axis[i]["mouse"].GetString());
                                    assert(_mouse_code_map.has(mouse_hash));
                                    Uint8 value;
                                    _mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_axis_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(axis[i].HasMember("gamepad"))
                            {
                                if(!axis[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(axis[i]["gamepad"].GetString());
                                    assert(_gamepad_code_map.has(gp_hash));
                                    Uint8 value;
                                    _gamepad_code_map.get(gp_hash, value);
                                    
                                    input_map->_axis_map._gamepad.set(value, name_hash);
                                }
                            }
                        }
                    }
                }
                
                free(load_data->buffer);
                free(load_data);
                
                return input_map_handle;
            }
            
            InputMapHandle null_handle;
            return null_handle;
        }
        
        PlayerContextHandle create_player()
        {
            return _player_contexts.create();
        }
        
         PlayerContextHandle get_default_player_context()
        {
            return _default_player_context;
        }
        
        void set_active_input_map(PlayerContextHandle player, InputMapHandle input_map)
        {
            if(_player_contexts.has(player))
            {
                PlayerContext* context = _player_contexts.get_ptr(player);
                
                if(context->_input_maps.has(input_map))
                {
                    context->_active = input_map;
                }
            }
        }
        
        void process_keyboard_input(Sint32 key, Sint32 scan_code, Uint32 action)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                // Check if Action
                if(action == SDL_KEYDOWN)
                {
                    if(input_map->_action_map._keyboard.has(key))
                    {
                        uint64_t action_hash;
                        input_map->_action_map._keyboard.get(key, action_hash);
                        
                        // Fire Event
                        InputActionEvent* event = new InputActionEvent(action);
                        terminus::EventHandler::queue_event(event);
                        
                        _keyboard_device.button_states[scan_code] = action;
                        
                        return;
                    }
                }
                
                // Check if State
                if(input_map->_state_map._keyboard.has(key))
                {
                    uint64_t state_hash;
                    input_map->_state_map._keyboard.get(key, state_hash);
                    
                    if(action == SDL_KEYDOWN)
                    {
                        // Fire Pressed Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 1);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    if(action == SDL_KEYUP)
                    {
                        // Fire Released Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 0);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    
                    _keyboard_device.button_states[scan_code] = action;
                }
                
                // Check if Axis Press
                if(action == SDL_KEYDOWN)
                {
                    // Check if Positive Axis
                    if(input_map->_axis_map._keyboard_pos.has(key))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._keyboard_pos.get(key, axis_hash);
                        
                        double last_value = _keyboard_device.button_axis_states[scan_code];
                        
                        // Fire Axis Positive Event
                        InputAxisEvent* event = new InputAxisEvent(axis_hash, 1.0, 1.0 - last_value);
                        terminus::EventHandler::queue_event(event);
                        
                        _keyboard_device.button_axis_states[scan_code] = 1.0;
                        
                        return;
                    }
                    
                    // Check if Negative Axis
                    if(input_map->_axis_map._keyboard_neg.has(key))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._keyboard_neg.get(key, axis_hash);
                        
                        double last_value = (double)_keyboard_device.button_axis_states[scan_code];
                        
                        // Fire Axis Negative Event
                        InputAxisEvent* event = new InputAxisEvent(axis_hash, -1.0, -1.0 - last_value);
                        terminus::EventHandler::queue_event(event);
                        
                        _keyboard_device.button_axis_states[scan_code] = -1.0;
                        
                        return;
                    }
                }
                
                // Check if Axis Release
                if(action == SDL_KEYUP)
                {
                    if(input_map->_axis_map._keyboard_pos.has(key))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._keyboard_pos.get(key, axis_hash);
                        
                        double last_value = (double)_keyboard_device.button_axis_states[scan_code];
                        
                        // Fire Axis Positive Event
                        InputAxisEvent* event = new InputAxisEvent(axis_hash, 0.0, 0.0 - last_value);
                        terminus::EventHandler::queue_event(event);
                        
                        _keyboard_device.button_axis_states[scan_code] = 0.0;
                        
                        return;
                    }
                    if(input_map->_axis_map._keyboard_neg.has(key))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._keyboard_neg.get(key, axis_hash);
                        
                        double last_value = (double)_keyboard_device.button_axis_states[scan_code];
                        
                        // Fire Axis Negative Event
                        InputAxisEvent* event = new InputAxisEvent(axis_hash, 0.0, 0.0 - last_value);
                        terminus::EventHandler::queue_event(event);
                        
                        _keyboard_device.button_axis_states[scan_code] = 0.0;
                        
                        return;
                    }
                }

            }
        }
        
        void process_mouse_button_input(Uint8 key, Uint32 action)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                // Check if Action
                if(action == SDL_KEYDOWN)
                {
                    if(input_map->_action_map._mouse.has(key))
                    {
                        uint64_t action_hash;
                        input_map->_action_map._mouse.get(key, action_hash);
                        // Fire Event
                        InputActionEvent* event = new InputActionEvent(action_hash);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                }
                
                // Check if State
                if(input_map->_state_map._mouse.has(key))
                {
                    uint64_t state_hash;
                    input_map->_state_map._mouse.get(key, state_hash);
                    
                    if(action == SDL_MOUSEBUTTONDOWN)
                    {
                        // Fire Pressed Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 1);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    if(action == SDL_MOUSEBUTTONUP)
                    {
                        // Fire Released Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 0);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                }
            }
            
            _mouse_device.button_states[key] = action;
        }
        
        void process_cursor_input(Sint32 xpos, Sint32 ypos, Sint32 xrel, Sint32 yrel)
        {
			//SDL_bool relative = SDL_GetRelativeMouseMode();

            if(_player_contexts.has(_default_player_context))
            {
                PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
                InputMap* input_map = context->_input_maps.get_ptr(context->_active);
				SDL_bool relative = SDL_GetRelativeMouseMode();

                if(input_map)
                {
                    if(input_map->_axis_map._mouse.has(MOUSE_AXIS_X))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._mouse.get(MOUSE_AXIS_X, axis_hash);
                        
                        double last_position = _mouse_device.x_position;
                        // Fire Mouse Axis Event
						InputAxisEvent* event;

						if(relative)
							event = new InputAxisEvent(axis_hash, xpos, xrel);
						else
							event = new InputAxisEvent(axis_hash, xpos, xpos - last_position);

                        terminus::EventHandler::queue_event(event);
                    }
                    if(input_map->_axis_map._mouse.has(MOUSE_AXIS_Y))
                    {
                        uint64_t axis_hash;
                        input_map->_axis_map._mouse.get(MOUSE_AXIS_Y, axis_hash);
                        
                        double last_position = _mouse_device.y_position;
                        // Fire Mouse Axis Event
						InputAxisEvent* event;

						if (relative)
							event = new InputAxisEvent(axis_hash, ypos, yrel);
						else
							event = new InputAxisEvent(axis_hash, ypos, ypos - last_position);

                        terminus::EventHandler::queue_event(event);
                    }
                }
            }
            
			//if (!relative)
			{
				_mouse_device.x_position = xpos;
				_mouse_device.y_position = ypos;
			}
        }
        
        void process_mouse_wheel_input(Uint32 value)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                if(input_map->_axis_map._mouse.has(MOUSE_WHEEL))
                {
                    uint64_t axis_hash;
                    input_map->_axis_map._mouse.get(MOUSE_WHEEL, axis_hash);
                    
                    Uint32 last_value = _mouse_device.wheel;
                    InputAxisEvent* event = new InputAxisEvent(axis_hash, value, value - last_value);
                    terminus::EventHandler::queue_event(event);
                }
            }
        }
        
        void process_window_events(SDL_Event& event)
        {
            switch (event.type)
            {
                case SDL_MOUSEWHEEL:
                    process_mouse_wheel_input(event.wheel.y);
                    break;
                    
                case SDL_MOUSEMOTION:
                    process_cursor_input(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    process_mouse_button_input(event.button.button, event.type);
                    break;
                    
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                {
                    if(event.key.repeat == 0)
                        process_keyboard_input(event.key.keysym.sym, event.key.keysym.scancode, event.type);
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
}
