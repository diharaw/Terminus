#include <input/input_handler.h>
#include <input/input_map.h>
#include <platform/platform.h>
#include <core/Event/event.h>
#include <core/Event/event_handler.h>
#include <utility/murmur_hash.h>
#include <resource/text_loader.h>
#include <io/filesystem.h>
#include <core/types.h>
#include <core/context.h>

#include <vector>

#define MAX_PLAYERS 8

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
            Platform* platform = context::get_platform();
            
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
                                    assert(platform->_keyboard_code_map.has(kb_hash));
                                    uint32_t value;
                                    platform->_keyboard_code_map.get(kb_hash, value);
                                    
                                    input_map->_action_map._keyboard.set(value, name_hash);
                                }
                            }
                            
                            if(actions[i].HasMember("mouse"))
                            {
                                if(!actions[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(actions[i]["mouse"].GetString());
                                    assert(platform->_mouse_code_map.has(mouse_hash));
                                    uint8_t value;
                                    platform->_mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_action_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(actions[i].HasMember("gamepad"))
                            {
                                if(!actions[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(actions[i]["gamepad"].GetString());
                                    assert(platform->_gamepad_code_map.has(gp_hash));
                                    uint8_t value;
                                    platform->_gamepad_code_map.get(gp_hash, value);
                                    
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
                                    assert(platform->_keyboard_code_map.has(kb_hash));
                                    uint32_t value;
                                    platform->_keyboard_code_map.get(kb_hash, value);
                                    
                                    input_map->_state_map._keyboard.set(value, name_hash);
                                }
                            }
                            
                            if(states[i].HasMember("mouse"))
                            {
                                if(!states[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(states[i]["mouse"].GetString());
                                    assert(platform->_mouse_code_map.has(mouse_hash));
                                    uint8_t value;
                                    platform->_mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_state_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(states[i].HasMember("gamepad"))
                            {
                                if(!states[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(states[i]["gamepad"].GetString());
                                    assert(platform->_gamepad_code_map.has(gp_hash));
                                    uint8_t value;
                                    platform->_gamepad_code_map.get(gp_hash, value);
                                    
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
                                        assert(platform->_keyboard_code_map.has(kb_hash));
                                        uint32_t value;
                                        platform->_keyboard_code_map.get(kb_hash, value);
                                        
                                        input_map->_axis_map._keyboard_pos.set(value, name_hash);
                                    }
                                    
                                    // negative value
                                    {
                                        uint64_t kb_hash = HASH(axis[i]["keyboard_neg"].GetString());
                                        assert(platform->_keyboard_code_map.has(kb_hash));
                                        uint32_t value;
                                        platform->_keyboard_code_map.get(kb_hash, value);
                                        
                                        input_map->_axis_map._keyboard_neg.set(value, name_hash);
                                    }
                                }
                            }
                            
                            if(axis[i].HasMember("mouse"))
                            {
                                if(!axis[i]["mouse"].IsNull())
                                {
                                    uint64_t mouse_hash = HASH(axis[i]["mouse"].GetString());
                                    assert(platform->_mouse_code_map.has(mouse_hash));
                                    uint8_t value;
                                    platform->_mouse_code_map.get(mouse_hash, value);
                                    
                                    input_map->_axis_map._mouse.set(value, name_hash);
                                }
                            }
                            
                            if(axis[i].HasMember("gamepad"))
                            {
                                if(!axis[i]["gamepad"].IsNull())
                                {
                                    uint64_t gp_hash = HASH(axis[i]["gamepad"].GetString());
                                    assert(platform->_gamepad_code_map.has(gp_hash));
                                    uint8_t value;
                                    platform->_gamepad_code_map.get(gp_hash, value);
                                    
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
        
        void process_keyboard_input(uint32_t key, uint32_t scan_code, uint32_t action)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                // Check if Action
                if(action == InputActionType::KEY_DOWN)
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
                    
                    if(action == InputActionType::KEY_DOWN)
                    {
                        // Fire Pressed Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 1);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    if(action == InputActionType::KEY_UP)
                    {
                        // Fire Released Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 0);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    
                    _keyboard_device.button_states[scan_code] = action;
                }
                
                // Check if Axis Press
                if(action == InputActionType::KEY_DOWN)
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
                if(action == InputActionType::KEY_UP)
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
        
        void process_mouse_button_input(uint8_t key, uint32_t action)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                // Check if Action
                if(action == InputActionType::MOUSE_BUTTON_DOWN)
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
                    
                    if(action == InputActionType::MOUSE_BUTTON_DOWN)
                    {
                        // Fire Pressed Event
                        InputStateEvent* event = new InputStateEvent(state_hash, 1);
                        terminus::EventHandler::queue_event(event);
                        
                        return;
                    }
                    if(action == InputActionType::MOUSE_BUTTON_UP)
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
        
        void process_cursor_input(int32_t xpos, int32_t ypos, int32_t xrel, int32_t yrel, bool relative)
        {
			//SDL_bool relative = SDL_GetRelativeMouseMode();

            if(_player_contexts.has(_default_player_context))
            {
                PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
                InputMap* input_map = context->_input_maps.get_ptr(context->_active);

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
        
        void process_mouse_wheel_input(uint32_t value)
        {
            PlayerContext* context = _player_contexts.get_ptr(_default_player_context);
            InputMap* input_map = context->_input_maps.get_ptr(context->_active);
            
            if(input_map)
            {
                if(input_map->_axis_map._mouse.has(MOUSE_WHEEL))
                {
                    uint64_t axis_hash;
                    input_map->_axis_map._mouse.get(MOUSE_WHEEL, axis_hash);
                    
                    uint32_t last_value = _mouse_device.wheel;
                    InputAxisEvent* event = new InputAxisEvent(axis_hash, value, value - last_value);
                    terminus::EventHandler::queue_event(event);
                }
            }
        }
    }
}
