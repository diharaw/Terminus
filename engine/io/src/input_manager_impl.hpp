#pragma once

#include <io/include/input_manager.hpp>
#include <io/src/input_map_impl.hpp>
#include <event/include/event_manager.hpp>
#include <memory/src/buffer_pool_allocator.hpp>
#include <stl/include/packed_array.hpp>
#include <array>
#include <SDL.h>

TE_BEGIN_TERMINUS_NAMESPACE

class InputManagerImpl : public InputManager
{
private:
    struct GamepadEntry
    {
        SDL_GameController* controller;
        SDL_Joystick*       joystick;
        SDL_JoystickID      id;
    };
    
    struct PlayerEntry
    {
        uint32_t  gamepad_handle;
        InputMapImpl* map;
    };
    
    double m_button_axis_states[TE_KEYBOARD_NUM_BUTTONS];
    PackedArray<GamepadEntry, TE_MAX_GAMEPADS> m_gamepads;
    PackedArray<PlayerEntry, TE_MAX_PLAYERS> m_players;
    uint8_t* m_keyboard_states;
    uint32_t m_default_player;
    double m_mouse_last_pos_x;
    double m_mouse_last_pos_y;
    double m_mouse_last_pos_wheel;
    
public:
    InputManagerImpl();
    ~InputManagerImpl();
    void initialize();
    void process_gamepad_added(EventManager* event_manager, int32_t device_index);
    void process_gamepad_removed(EventManager* event_manager, int32_t device_index);
    void process_joystick_added(EventManager* event_manager, int32_t device_index);
    void process_joystick_removed(EventManager* event_manager, int32_t device_index);
    void process_mouse_motion(EventManager* event_manager, int32_t xpos, int32_t ypos, int32_t xrel, int32_t yrel, bool relative);
    void process_mouse_button(EventManager* event_manager, uint8_t key, uint32_t state);
    void process_mouse_wheel(EventManager* event_manager, uint32_t value);
    void process_keyboard_button(EventManager* event_manager, uint32_t scan_code, uint32_t state);
    void process_gamepad_axis(EventManager* event_manager, uint8_t axis, int16_t value, int32_t device_id);
    void process_gamepad_button(EventManager* event_manager, uint8_t button, uint8_t state, int32_t device_id);
    void process_joystick_axis(EventManager* event_manager, uint8_t axis, int32_t value, int32_t device_id);
    void process_joystick_button(EventManager* event_manager, uint8_t button, uint8_t state, int32_t device_id);
    void process_joystick_hat(EventManager* event_manager, uint8_t hat, uint8_t value, int32_t device_id);
    uint32_t create_player() override;
    uint32_t num_players() override;
    void     destroy_player(uint32_t player) override;
    void     set_input_map(InputMap* map, uint32_t player) override;
    uint32_t assign_gamepad(int32_t device_index, uint32_t player) override;
    void     resign_gamepad(uint32_t handle, uint32_t player) override;
    uint32_t assign_joystick(int32_t device_index, uint32_t player) override;
    void     resign_joystick(uint32_t handle, uint32_t player) override;
    uint32_t num_gamepad_device() override;
    uint32_t num_joystick_device() override;
    bool     keyboard_pressed(uint32_t code) override;
    bool     mouse_pressed(uint32_t code) override;
    double   mouse_axis(uint32_t code) override;
    bool     gamepad_pressed(uint32_t code, uint32_t handle) override;
    double   gamepad_axis(uint32_t code, uint32_t handle) override;
};

TE_END_TERMINUS_NAMESPACE
