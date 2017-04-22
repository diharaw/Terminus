#pragma once

#include <core/types.h>
#include <container/handle.h>
#include <input/player_context.h>

#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYBOARD_BUTTONS 512

namespace terminus
{    
    using PlayerContextHandle = Handle<PlayerContext>;
    
    enum InputActionType
    {
        KEY_DOWN = 0,
        KEY_UP,
        MOUSE_BUTTON_DOWN,
        MOUSE_BUTTON_UP
    };

    struct KeyboardButton
    {
        enum Codes
        {
            KB_UNKNOWN = 0,
            KB_A = 4,
            KB_B = 5,
            KB_C = 6,
            KB_D = 7,
            KB_E = 8,
            KB_F = 9,
            KB_G = 10,
            KB_H = 11,
            KB_I = 12,
            KB_J = 13,
            KB_K = 14,
            KB_L = 15,
            KB_M = 16,
            KB_N = 17,
            KB_O = 18,
            KB_P = 19,
            KB_Q = 20,
            KB_R = 21,
            KB_S = 22,
            KB_T = 23,
            KB_U = 24,
            KB_V = 25,
            KB_W = 26,
            KB_X = 27,
            KB_Y = 28,
            KB_Z = 29,
            KB_NUM_1 = 30,
            KB_NUM_2 = 31,
            KB_NUM_3 = 32,
            KB_NUM_4 = 33,
            KB_NUM_5 = 34,
            KB_NUM_6 = 35,
            KB_NUM_7 = 36,
            KB_NUM_8 = 37,
            KB_NUM_9 = 38,
            KB_NUM_0 = 39,
            KB_RETURN = 40,
            KB_ESCAPE = 41,
            KB_BACKSPACE = 42,
            KB_TAB = 43,
            KB_SPACE = 44,
            KB_MINUS = 45,
            KB_EQUALS = 46,
            KB_LEFTBRACKET = 47,
            KB_RIGHTBRACKET = 48,
            KB_BACKSLASH = 49,
            KB_NONUSHASH = 50,
            KB_SEMICOLON = 51,
            KB_APOSTROPHE = 52,
            KB_GRAVE = 53,
            KB_COMMA = 54,
            KB_PERIOD = 55,
            KB_SLASH = 56,
            KB_CAPSLOCK = 57,
            KB_F1 = 58,
            KB_F2 = 59,
            KB_F3 = 60,
            KB_F4 = 61,
            KB_F5 = 62,
            KB_F6 = 63,
            KB_F7 = 64,
            KB_F8 = 65,
            KB_F9 = 66,
            KB_F10 = 67,
            KB_F11 = 68,
            KB_F12 = 69,
            KB_PRINTSCREEN = 70,
            KB_SCROLLLOCK = 71,
            KB_PAUSE = 72,
            KB_INSERT = 73,
            KB_HOME = 74,
            KB_PAGEUP = 75,
            KB_DELETE = 76,
            KB_END = 77,
            KB_PAGEDOWN = 78,
            KB_RIGHT = 79,
            KB_LEFT = 80,
            KB_DOWN = 81,
            KB_UP = 82,
            KB_NUMLOCKCLEAR = 83,
            KB_KP_DIVIDE = 84,
            KB_KP_MULTIPLY = 85,
            KB_KP_MINUS = 86,
            KB_KP_PLUS = 87,
            KB_KP_ENTER = 88,
            KB_KP_1 = 89,
            KB_KP_2 = 90,
            KB_KP_3 = 91,
            KB_KP_4 = 92,
            KB_KP_5 = 93,
            KB_KP_6 = 94,
            KB_KP_7 = 95,
            KB_KP_8 = 96,
            KB_KP_9 = 97,
            KB_KP_0 = 98,
            KB_KP_PERIOD = 99,
            KB_NONUSBACKSLASH = 100,
            KB_APPLICATION = 101,
            KB_POWER = 102,
            KB_KP_EQUALS = 103,
            KB_F13 = 104,
            KB_F14 = 105,
            KB_F15 = 106,
            KB_F16 = 107,
            KB_F17 = 108,
            KB_F18 = 109,
            KB_F19 = 110,
            KB_F20 = 111,
            KB_F21 = 112,
            KB_F22 = 113,
            KB_F23 = 114,
            KB_F24 = 115,
            KB_EXECUTE = 116,
            KB_HELP = 117,
            KB_MENU = 118,
            KB_SELECT = 119,
            KB_STOP = 120,
            KB_AGAIN = 121,
            KB_UNDO = 122,
            KB_CUT = 123,
            KB_COPY = 124,
            KB_PASTE = 125,
            KB_FIND = 126,
            KB_MUTE = 127,
            KB_VOLUMEUP = 128,
            KB_VOLUMEDOWN = 129,
            KB_KP_COMMA = 133,
            KB_KP_EQUALSAS400 = 134,
            KB_INTERNATIONAL1 = 135,
            KB_INTERNATIONAL2 = 136,
            KB_INTERNATIONAL3 = 137,
            KB_INTERNATIONAL4 = 138,
            KB_INTERNATIONAL5 = 139,
            KB_INTERNATIONAL6 = 140,
            KB_INTERNATIONAL7 = 141,
            KB_INTERNATIONAL8 = 142,
            KB_INTERNATIONAL9 = 143,
            KB_LANG1 = 144, 
            KB_LANG2 = 145, 
            KB_LANG3 = 146, 
            KB_LANG4 = 147, 
            KB_LANG5 = 148, 
            KB_LANG6 = 149, 
            KB_LANG7 = 150, 
            KB_LANG8 = 151, 
            KB_LANG9 = 152,
            KB_ALTERASE = 153,
            KB_SYSREQ = 154,
            KB_CANCEL = 155,
            KB_CLEAR = 156,
            KB_PRIOR = 157,
            KB_RETURN2 = 158,
            KB_SEPARATOR = 159,
            KB_OUT = 160,
            KB_OPER = 161,
            KB_CLEARAGAIN = 162,
            KB_CRSEL = 163,
            KB_EXSEL = 164,
            KB_KP_00 = 176,
            KB_KP_000 = 177,
            KB_THOUSANDSSEPARATOR = 178,
            KB_DECIMALSEPARATOR = 179,
            KB_CURRENCYUNIT = 180,
            KB_CURRENCYSUBUNIT = 181,
            KB_KP_LEFTPAREN = 182,
            KB_KP_RIGHTPAREN = 183,
            KB_KP_LEFTBRACE = 184,
            KB_KP_RIGHTBRACE = 185,
            KB_KP_TAB = 186,
            KB_KP_BACKSPACE = 187,
            KB_KP_A = 188,
            KB_KP_B = 189,
            KB_KP_C = 190,
            KB_KP_D = 191,
            KB_KP_E = 192,
            KB_KP_F = 193,
            KB_KP_XOR = 194,
            KB_KP_POWER = 195,
            KB_KP_PERCENT = 196,
            KB_KP_LESS = 197,
            KB_KP_GREATER = 198,
            KB_KP_AMPERSAND = 199,
            KB_KP_DBLAMPERSAND = 200,
            KB_KP_VERTICALBAR = 201,
            KB_KP_DBLVERTICALBAR = 202,
            KB_KP_COLON = 203,
            KB_KP_HASH = 204,
            KB_KP_SPACE = 205,
            KB_KP_AT = 206,
            KB_KP_EXCLAM = 207,
            KB_KP_MEMSTORE = 208,
            KB_KP_MEMRECALL = 209,
            KB_KP_MEMCLEAR = 210,
            KB_KP_MEMADD = 211,
            KB_KP_MEMSUBTRACT = 212,
            KB_KP_MEMMULTIPLY = 213,
            KB_KP_MEMDIVIDE = 214,
            KB_KP_PLUSMINUS = 215,
            KB_KP_CLEAR = 216,
            KB_KP_CLEARENTRY = 217,
            KB_KP_BINARY = 218,
            KB_KP_OCTAL = 219,
            KB_KP_DECIMAL = 220,
            KB_KP_HEXADECIMAL = 221,
            KB_LCTRL = 224,
            KB_LSHIFT = 225,
            KB_LALT = 226,
            KB_LGUI = 227,
            KB_RCTRL = 228,
            KB_RSHIFT = 229,
            KB_RALT = 230,
            KB_RGUI = 231,
            KB_MODE = 257,
            KB_AUDIONEXT = 258,
            KB_AUDIOPREV = 259,
            KB_AUDIOSTOP = 260,
            KB_AUDIOPLAY = 261,
            KB_AUDIOMUTE = 262,
            KB_MEDIASELECT = 263,
            KB_WWW = 264,
            KB_MAIL = 265,
            KB_CALCULATOR = 266,
            KB_COMPUTER = 267,
            KB_AC_SEARCH = 268,
            KB_AC_HOME = 269,
            KB_AC_BACK = 270,
            KB_AC_FORWARD = 271,
            KB_AC_STOP = 272,
            KB_AC_REFRESH = 273,
            KB_AC_BOOKMARKS = 274,
            KB_BRIGHTNESSDOWN = 275,
            KB_BRIGHTNESSUP = 276,
            KB_DISPLAYSWITCH = 277,
            KB_KBDILLUMTOGGLE = 278,
            KB_KBDILLUMDOWN = 279,
            KB_KBDILLUMUP = 280,
            KB_EJECT = 281,
            KB_SLEEP = 282,
            KB_APP1 = 283,
            KB_APP2 = 284,
            KB_NUM_BUTTONS = 512
        };
    };
        
    struct MouseButton
    {
        enum Codes
        {
            BUTTON_1       =  1       ,
            BUTTON_2       =  2       ,
            BUTTON_3       =  3       ,
            BUTTON_4       =  4       ,
            BUTTON_5       =  5       ,
            BUTTON_6       =  6       ,
            BUTTON_7       =  7       ,
            BUTTON_8       =  8       ,
            LAST           =  BUTTON_8,
            LEFT           =  BUTTON_1,
            MIDDLE         =  BUTTON_2,
            RIGHT          =  BUTTON_3,
            NUM_BUTTONS    =  8       
        };
    };

    struct MouseAxis
    {
        enum Codes
        {
            X         =  9       ,
            Y         =  10      ,
            WHEEL     =  11    
        };
    };
    
    struct GamepadButton
    {
        enum Codes
        {
            INVALID         = -1  ,
            A               =  0  ,
            B               =  1  ,
            X               =  2  ,
            Y               =  3  ,
            BACK            =  4  ,
            GUIDE           =  5  ,
            START           =  6  ,
            LEFT_STICK      =  7  ,
            RIGHT_STICK     =  8  ,
            LEFT_SHOULDER   =  9  ,
            RIGHT_SHOULDER  =  10 ,
            DPAD_UP         =  11 ,
            DPAD_DOWN       =  12 ,
            DPAD_LEFT       =  13 ,
            DPAD_RIGHT      =  14 ,
            MAX             =  15
        };
    };

    struct GamepadAxis
    {
        enum Codes
        {
            INVALID           = -1  ,
            LEFT_STICK_X      =  0  ,
            LEFT_STICK_Y      =  1  ,
            RIGHT_STICK_X     =  2  ,
            RIGHT_STICK_Y     =  3  ,
            TRIGGER_LEFT      =  4  ,
            TRIGGER_RIGHT     =  5  ,
            MAX               =  6
        };
    };
    
    namespace input_handler
    {
        struct GamepadDevice
        {
            
        };
        
        struct MouseDevice
        {
            bool button_states[MAX_MOUSE_BUTTONS];
            uint32_t wheel;
            int x_position;
            int y_position;
        };
        
        struct KeyboardDevice
        {
            bool button_states[MAX_KEYBOARD_BUTTONS];
            double button_axis_states[MAX_KEYBOARD_BUTTONS];
        };
        
        extern void initialize();
        extern PlayerContextHandle create_player();
        extern PlayerContextHandle get_default_player_context();
        extern InputMapHandle load_input_map(PlayerContextHandle player, String name);
        extern void set_active_input_map(PlayerContextHandle player, InputMapHandle input_map);
        extern void process_keyboard_input(uint32_t scan_code, uint32_t action);
        extern void process_mouse_button_input(uint8_t Key, uint32_t action);
        extern void process_cursor_input(int32_t xpos, int32_t ypos, int32_t xrel, int32_t yrel, bool relative);
        extern void process_mouse_wheel_input(uint32_t value);
        extern MouseDevice& get_mouse_device();
        extern KeyboardDevice& get_keyboard_device();
        extern GamepadDevice& get_gamepad_device(PlayerContextHandle player);
    }

}
