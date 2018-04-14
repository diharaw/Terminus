#pragma once

#include <stdint.h>
#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>

enum ButtonState
{
    TE_BUTTON_STATE_PRESSED  = 0,
    TE_BUTTON_STATE_RELEASED = 1
};

enum KeyboardScancode
{
    TE_KEYBOARD_UNKNOWN = 0,
    TE_KEYBOARD_A = 4,
    TE_KEYBOARD_B = 5,
    TE_KEYBOARD_C = 6,
    TE_KEYBOARD_D = 7,
    TE_KEYBOARD_E = 8,
    TE_KEYBOARD_F = 9,
    TE_KEYBOARD_G = 10,
    TE_KEYBOARD_H = 11,
    TE_KEYBOARD_I = 12,
    TE_KEYBOARD_J = 13,
    TE_KEYBOARD_K = 14,
    TE_KEYBOARD_L = 15,
    TE_KEYBOARD_M = 16,
    TE_KEYBOARD_N = 17,
    TE_KEYBOARD_O = 18,
    TE_KEYBOARD_P = 19,
    TE_KEYBOARD_Q = 20,
    TE_KEYBOARD_R = 21,
    TE_KEYBOARD_S = 22,
    TE_KEYBOARD_T = 23,
    TE_KEYBOARD_U = 24,
    TE_KEYBOARD_V = 25,
    TE_KEYBOARD_W = 26,
    TE_KEYBOARD_X = 27,
    TE_KEYBOARD_Y = 28,
    TE_KEYBOARD_Z = 29,
    TE_KEYBOARD_NUM_1 = 30,
    TE_KEYBOARD_NUM_2 = 31,
    TE_KEYBOARD_NUM_3 = 32,
    TE_KEYBOARD_NUM_4 = 33,
    TE_KEYBOARD_NUM_5 = 34,
    TE_KEYBOARD_NUM_6 = 35,
    TE_KEYBOARD_NUM_7 = 36,
    TE_KEYBOARD_NUM_8 = 37,
    TE_KEYBOARD_NUM_9 = 38,
    TE_KEYBOARD_NUM_0 = 39,
    TE_KEYBOARD_RETURN = 40,
    TE_KEYBOARD_ESCAPE = 41,
    TE_KEYBOARD_BACKSPACE = 42,
    TE_KEYBOARD_TAB = 43,
    TE_KEYBOARD_SPACE = 44,
    TE_KEYBOARD_MINUS = 45,
    TE_KEYBOARD_EQUALS = 46,
    TE_KEYBOARD_LEFTBRACKET = 47,
    TE_KEYBOARD_RIGHTBRACKET = 48,
    TE_KEYBOARD_BACKSLASH = 49,
    TE_KEYBOARD_NONUSHASH = 50,
    TE_KEYBOARD_SEMICOLON = 51,
    TE_KEYBOARD_APOSTROPHE = 52,
    TE_KEYBOARD_GRAVE = 53,
    TE_KEYBOARD_COMMA = 54,
    TE_KEYBOARD_PERIOD = 55,
    TE_KEYBOARD_SLASH = 56,
    TE_KEYBOARD_CAPSLOCK = 57,
    TE_KEYBOARD_F1 = 58,
    TE_KEYBOARD_F2 = 59,
    TE_KEYBOARD_F3 = 60,
    TE_KEYBOARD_F4 = 61,
    TE_KEYBOARD_F5 = 62,
    TE_KEYBOARD_F6 = 63,
    TE_KEYBOARD_F7 = 64,
    TE_KEYBOARD_F8 = 65,
    TE_KEYBOARD_F9 = 66,
    TE_KEYBOARD_F10 = 67,
    TE_KEYBOARD_F11 = 68,
    TE_KEYBOARD_F12 = 69,
    TE_KEYBOARD_PRINTSCREEN = 70,
    TE_KEYBOARD_SCROLLLOCK = 71,
    TE_KEYBOARD_PAUSE = 72,
    TE_KEYBOARD_INSERT = 73,
    TE_KEYBOARD_HOME = 74,
    TE_KEYBOARD_PAGEUP = 75,
    TE_KEYBOARD_DELETE = 76,
    TE_KEYBOARD_END = 77,
    TE_KEYBOARD_PAGEDOWN = 78,
    TE_KEYBOARD_RIGHT = 79,
    TE_KEYBOARD_LEFT = 80,
    TE_KEYBOARD_DOWN = 81,
    TE_KEYBOARD_UP = 82,
    TE_KEYBOARD_NUMLOCKCLEAR = 83,
    TE_KEYBOARD_KP_DIVIDE = 84,
    TE_KEYBOARD_KP_MULTIPLY = 85,
    TE_KEYBOARD_KP_MINUS = 86,
    TE_KEYBOARD_KP_PLUS = 87,
    TE_KEYBOARD_KP_ENTER = 88,
    TE_KEYBOARD_KP_1 = 89,
    TE_KEYBOARD_KP_2 = 90,
    TE_KEYBOARD_KP_3 = 91,
    TE_KEYBOARD_KP_4 = 92,
    TE_KEYBOARD_KP_5 = 93,
    TE_KEYBOARD_KP_6 = 94,
    TE_KEYBOARD_KP_7 = 95,
    TE_KEYBOARD_KP_8 = 96,
    TE_KEYBOARD_KP_9 = 97,
    TE_KEYBOARD_KP_0 = 98,
    TE_KEYBOARD_KP_PERIOD = 99,
    TE_KEYBOARD_NONUSBACKSLASH = 100,
    TE_KEYBOARD_APPLICATION = 101,
    TE_KEYBOARD_POWER = 102,
    TE_KEYBOARD_KP_EQUALS = 103,
    TE_KEYBOARD_F13 = 104,
    TE_KEYBOARD_F14 = 105,
    TE_KEYBOARD_F15 = 106,
    TE_KEYBOARD_F16 = 107,
    TE_KEYBOARD_F17 = 108,
    TE_KEYBOARD_F18 = 109,
    TE_KEYBOARD_F19 = 110,
    TE_KEYBOARD_F20 = 111,
    TE_KEYBOARD_F21 = 112,
    TE_KEYBOARD_F22 = 113,
    TE_KEYBOARD_F23 = 114,
    TE_KEYBOARD_F24 = 115,
    TE_KEYBOARD_EXECUTE = 116,
    TE_KEYBOARD_HELP = 117,
    TE_KEYBOARD_MENU = 118,
    TE_KEYBOARD_SELECT = 119,
    TE_KEYBOARD_STOP = 120,
    TE_KEYBOARD_AGAIN = 121,
    TE_KEYBOARD_UNDO = 122,
    TE_KEYBOARD_CUT = 123,
    TE_KEYBOARD_COPY = 124,
    TE_KEYBOARD_PASTE = 125,
    TE_KEYBOARD_FIND = 126,
    TE_KEYBOARD_MUTE = 127,
    TE_KEYBOARD_VOLUMEUP = 128,
    TE_KEYBOARD_VOLUMEDOWN = 129,
    TE_KEYBOARD_KP_COMMA = 133,
    TE_KEYBOARD_KP_EQUALSAS400 = 134,
    TE_KEYBOARD_INTERNATIONAL1 = 135,
    TE_KEYBOARD_INTERNATIONAL2 = 136,
    TE_KEYBOARD_INTERNATIONAL3 = 137,
    TE_KEYBOARD_INTERNATIONAL4 = 138,
    TE_KEYBOARD_INTERNATIONAL5 = 139,
    TE_KEYBOARD_INTERNATIONAL6 = 140,
    TE_KEYBOARD_INTERNATIONAL7 = 141,
    TE_KEYBOARD_INTERNATIONAL8 = 142,
    TE_KEYBOARD_INTERNATIONAL9 = 143,
    TE_KEYBOARD_LANG1 = 144,
    TE_KEYBOARD_LANG2 = 145,
    TE_KEYBOARD_LANG3 = 146,
    TE_KEYBOARD_LANG4 = 147,
    TE_KEYBOARD_LANG5 = 148,
    TE_KEYBOARD_LANG6 = 149,
    TE_KEYBOARD_LANG7 = 150,
    TE_KEYBOARD_LANG8 = 151,
    TE_KEYBOARD_LANG9 = 152,
    TE_KEYBOARD_ALTERASE = 153,
    TE_KEYBOARD_SYSREQ = 154,
    TE_KEYBOARD_CANCEL = 155,
    TE_KEYBOARD_CLEAR = 156,
    TE_KEYBOARD_PRIOR = 157,
    TE_KEYBOARD_RETURN2 = 158,
    TE_KEYBOARD_SEPARATOR = 159,
    TE_KEYBOARD_OUT = 160,
    TE_KEYBOARD_OPER = 161,
    TE_KEYBOARD_CLEARAGAIN = 162,
    TE_KEYBOARD_CRSEL = 163,
    TE_KEYBOARD_EXSEL = 164,
    TE_KEYBOARD_KP_00 = 176,
    TE_KEYBOARD_KP_000 = 177,
    TE_KEYBOARD_THOUSANDSSEPARATOR = 178,
    TE_KEYBOARD_DECIMALSEPARATOR = 179,
    TE_KEYBOARD_CURRENCYUNIT = 180,
    TE_KEYBOARD_CURRENCYSUBUNIT = 181,
    TE_KEYBOARD_KP_LEFTPAREN = 182,
    TE_KEYBOARD_KP_RIGHTPAREN = 183,
    TE_KEYBOARD_KP_LEFTBRACE = 184,
    TE_KEYBOARD_KP_RIGHTBRACE = 185,
    TE_KEYBOARD_KP_TAB = 186,
    TE_KEYBOARD_KP_BACKSPACE = 187,
    TE_KEYBOARD_KP_A = 188,
    TE_KEYBOARD_KP_B = 189,
    TE_KEYBOARD_KP_C = 190,
    TE_KEYBOARD_KP_D = 191,
    TE_KEYBOARD_KP_E = 192,
    TE_KEYBOARD_KP_F = 193,
    TE_KEYBOARD_KP_XOR = 194,
    TE_KEYBOARD_KP_POWER = 195,
    TE_KEYBOARD_KP_PERCENT = 196,
    TE_KEYBOARD_KP_LESS = 197,
    TE_KEYBOARD_KP_GREATER = 198,
    TE_KEYBOARD_KP_AMPERSAND = 199,
    TE_KEYBOARD_KP_DBLAMPERSAND = 200,
    TE_KEYBOARD_KP_VERTICALBAR = 201,
    TE_KEYBOARD_KP_DBLVERTICALBAR = 202,
    TE_KEYBOARD_KP_COLON = 203,
    TE_KEYBOARD_KP_HASH = 204,
    TE_KEYBOARD_KP_SPACE = 205,
    TE_KEYBOARD_KP_AT = 206,
    TE_KEYBOARD_KP_EXCLAM = 207,
    TE_KEYBOARD_KP_MEMSTORE = 208,
    TE_KEYBOARD_KP_MEMRECALL = 209,
    TE_KEYBOARD_KP_MEMCLEAR = 210,
    TE_KEYBOARD_KP_MEMADD = 211,
    TE_KEYBOARD_KP_MEMSUBTRACT = 212,
    TE_KEYBOARD_KP_MEMMULTIPLY = 213,
    TE_KEYBOARD_KP_MEMDIVIDE = 214,
    TE_KEYBOARD_KP_PLUSMINUS = 215,
    TE_KEYBOARD_KP_CLEAR = 216,
    TE_KEYBOARD_KP_CLEARENTRY = 217,
    TE_KEYBOARD_KP_BINARY = 218,
    TE_KEYBOARD_KP_OCTAL = 219,
    TE_KEYBOARD_KP_DECIMAL = 220,
    TE_KEYBOARD_KP_HEXADECIMAL = 221,
    TE_KEYBOARD_LCTRL = 224,
    TE_KEYBOARD_LSHIFT = 225,
    TE_KEYBOARD_LALT = 226,
    TE_KEYBOARD_LGUI = 227,
    TE_KEYBOARD_RCTRL = 228,
    TE_KEYBOARD_RSHIFT = 229,
    TE_KEYBOARD_RALT = 230,
    TE_KEYBOARD_RGUI = 231,
    TE_KEYBOARD_MODE = 257,
    TE_KEYBOARD_AUDIONEXT = 258,
    TE_KEYBOARD_AUDIOPREV = 259,
    TE_KEYBOARD_AUDIOSTOP = 260,
    TE_KEYBOARD_AUDIOPLAY = 261,
    TE_KEYBOARD_AUDIOMUTE = 262,
    TE_KEYBOARD_MEDIASELECT = 263,
    TE_KEYBOARD_WWW = 264,
    TE_KEYBOARD_MAIL = 265,
    TE_KEYBOARD_CALCULATOR = 266,
    TE_KEYBOARD_COMPUTER = 267,
    TE_KEYBOARD_AC_SEARCH = 268,
    TE_KEYBOARD_AC_HOME = 269,
    TE_KEYBOARD_AC_BACK = 270,
    TE_KEYBOARD_AC_FORWARD = 271,
    TE_KEYBOARD_AC_STOP = 272,
    TE_KEYBOARD_AC_REFRESH = 273,
    TE_KEYBOARD_AC_BOOKMARKS = 274,
    TE_KEYBOARD_BRIGHTNESSDOWN = 275,
    TE_KEYBOARD_BRIGHTNESSUP = 276,
    TE_KEYBOARD_DISPLAYSWITCH = 277,
    TE_KEYBOARD_KBDILLUMTOGGLE = 278,
    TE_KEYBOARD_KBDILLUMDOWN = 279,
    TE_KEYBOARD_KBDILLUMUP = 280,
    TE_KEYBOARD_EJECT = 281,
    TE_KEYBOARD_SLEEP = 282,
    TE_KEYBOARD_APP1 = 283,
    TE_KEYBOARD_APP2 = 284,
    TE_KEYBOARD_NUM_BUTTONS = 512
};

enum MouseButton
{
    TE_MOUSE_BUTTON_1       =  1       ,
    TE_MOUSE_BUTTON_2       =  2       ,
    TE_MOUSE_BUTTON_3       =  3       ,
    TE_MOUSE_BUTTON_4       =  4       ,
    TE_MOUSE_BUTTON_5       =  5       ,
    TE_MOUSE_BUTTON_6       =  6       ,
    TE_MOUSE_BUTTON_7       =  7       ,
    TE_MOUSE_BUTTON_8       =  8       ,
    TE_MOUSE_LAST           =  TE_MOUSE_BUTTON_8,
    TE_MOUSE_LEFT           =  TE_MOUSE_BUTTON_1,
    TE_MOUSE_MIDDLE         =  TE_MOUSE_BUTTON_2,
    TE_MOUSE_RIGHT          =  TE_MOUSE_BUTTON_3,
    TE_MOUSE_NUM_BUTTONS    =  8
};

enum MouseAxis
{
    TE_MOUSE_AXIS_X         =  9       ,
    TE_MOUSE_AXIS_Y         =  10      ,
    TE_MOUSE_AXIS_WHEEL     =  11
};

enum GamepadButton
{
    TE_GAMEPAD_INVALID         = -1  ,
    TE_GAMEPAD_A               =  0  ,
    TE_GAMEPAD_B               =  1  ,
    TE_GAMEPAD_X               =  2  ,
    TE_GAMEPAD_Y               =  3  ,
    TE_GAMEPAD_BACK            =  4  ,
    TE_GAMEPAD_GUIDE           =  5  ,
    TE_GAMEPAD_START           =  6  ,
    TE_GAMEPAD_LEFT_STICK      =  7  ,
    TE_GAMEPAD_RIGHT_STICK     =  8  ,
    TE_GAMEPAD_LEFT_SHOULDER   =  9  ,
    TE_GAMEPAD_RIGHT_SHOULDER  =  10 ,
    TE_GAMEPAD_DPAD_UP         =  11 ,
    TE_GAMEPAD_DPAD_DOWN       =  12 ,
    TE_GAMEPAD_DPAD_LEFT       =  13 ,
    TE_GAMEPAD_DPAD_RIGHT      =  14 ,
    TE_GAMEPAD_MAX             =  15
};

enum GamepadAxis
{
    TE_GAMEPAD_AXIS_INVALID = -1,
    TE_GAMEPAD_AXIS_LEFTX,
    TE_GAMEPAD_AXIS_LEFTY,
    TE_GAMEPAD_AXIS_RIGHTX,
    TE_GAMEPAD_AXIS_RIGHTY,
    TE_GAMEPAD_AXIS_TRIGGERLEFT,
    TE_GAMEPAD_AXIS_TRIGGERRIGHT,
    TE_GAMEPAD_AXIS_MAX
};

enum JoystickAxis
{
    TE_JOYSTICK_AXIS_X = 0,
    TE_JOYSTICK_AXIS_Y = 1,
};

enum JoystickHat
{
    TE_HAT_CENTERED   = 0x00,
    TE_HAT_UP         = 0x01,
    TE_HAT_RIGHT      = 0x02,
    TE_HAT_DOWN       = 0x04,
    TE_HAT_LEFT       = 0x08,
    TE_HAT_RIGHTUP    = (TE_HAT_RIGHT|TE_HAT_UP),
    TE_HAT_RIGHTDOWN  = (TE_HAT_RIGHT|TE_HAT_DOWN),
    TE_HAT_LEFTUP     = (TE_HAT_LEFT|TE_HAT_UP),
    TE_HAT_LEFTDOWN   = (TE_HAT_LEFT|TE_HAT_DOWN)
};
