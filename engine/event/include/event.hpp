#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/terminus_types.hpp>
#include <stl/include/string_hash.hpp>

enum EventType
{
    TE_EVENT_AXIS_INPUT     = 0,
    TE_EVENT_STATE_INPUT    = 1,
    TE_EVENT_ACTION_INPUT   = 2,
    TE_EVENT_SCENE_LOADED   = 3,
    TE_EVENT_SCENE_UNLOADED = 4,
    TE_EVENT_TRIGGER        = 5
};

TE_BEGIN_TERMINUS_NAMESPACE

struct WindowEvent
{
    uint8_t  type;
    uint32_t id;
};

struct KeyboardEvent
{
    uint32_t scancode;
    uint8_t  state;
};

struct MouseMotionEvent
{
    int32_t xpos;
    int32_t ypos;
    int32_t xrel;
    int32_t yrel;
    bool    relative;
};

struct MouseButtonEvent
{
    uint8_t button;
};

struct MouseWheelEvent
{
    uint32_t value;
};

struct JoyAxisEvent
{

};

struct JoyBallEvent
{

};

struct JoyHatEvent
{

};

struct JoyButtonEvent
{

};

struct JoyDeviceEvent
{

};

struct ControllerAxisEvent
{

};

struct ControllerButtonEvent
{
};

struct ControllerDeviceEvent
{

};

struct InputAxisEvent
{
    StringHash hash;
    double     value;
    double     delta;
};

struct InputStateEvent
{
    StringHash hash;
    uint32_t   state;
};

struct InputActionEvent
{
    StringHash hash;
};

struct Event
{
	uint32_t type;

	union EventData
	{
        // OS Events
		WindowEvent			  window;
		KeyboardEvent		  keyboard;
		MouseMotionEvent	  mouse_motion;
		MouseButtonEvent	  mouse_button;
		MouseWheelEvent		  mouse_wheel;
		JoyAxisEvent		  joy_axis;
		JoyBallEvent		  joy_ball;
		JoyHatEvent			  joy_hat;
		JoyButtonEvent		  joy_button;
		JoyDeviceEvent		  joy_device;
		ControllerAxisEvent	  controller_axis;
		ControllerButtonEvent controller_button;
		ControllerDeviceEvent controlller_device;
        
        // User Events
        InputAxisEvent        input_axis;
        InputStateEvent       input_state;
        InputActionEvent      input_action;
        
        EventData()
        {
            memset(this, 0, sizeof(EventData));
        }
                   
	} data;
};

TE_END_TERMINUS_NAMESPACE
