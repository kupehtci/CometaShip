#ifndef COMETAKEYS_H
#define COMETAKEYS_H

// Cometa Keys file define a set of keys that can be used for Key input handling.
//

// Keyboard keys
#define COMETA_KEY_SPACE              32
#define COMETA_KEY_APOSTROPHE         39  /* ' */
#define COMETA_KEY_COMMA              44  /* , */
#define COMETA_KEY_MINUS              45  /* - */
#define COMETA_KEY_PERIOD             46  /* . */
#define COMETA_KEY_SLASH              47  /* / */
#define COMETA_KEY_0                  48
#define COMETA_KEY_1                  49
#define COMETA_KEY_2                  50
#define COMETA_KEY_3                  51
#define COMETA_KEY_4                  52
#define COMETA_KEY_5                  53
#define COMETA_KEY_6                  54
#define COMETA_KEY_7                  55
#define COMETA_KEY_8                  56
#define COMETA_KEY_9                  57
#define COMETA_KEY_SEMICOLON          59  /* ; */
#define COMETA_KEY_EQUAL              61  /* = */
#define COMETA_KEY_A                  65
#define COMETA_KEY_B                  66
#define COMETA_KEY_C                  67
#define COMETA_KEY_D                  68
#define COMETA_KEY_E                  69
#define COMETA_KEY_F                  70
#define COMETA_KEY_G                  71
#define COMETA_KEY_H                  72
#define COMETA_KEY_I                  73
#define COMETA_KEY_J                  74
#define COMETA_KEY_K                  75
#define COMETA_KEY_L                  76
#define COMETA_KEY_M                  77
#define COMETA_KEY_N                  78
#define COMETA_KEY_O                  79
#define COMETA_KEY_P                  80
#define COMETA_KEY_Q                  81
#define COMETA_KEY_R                  82
#define COMETA_KEY_S                  83
#define COMETA_KEY_T                  84
#define COMETA_KEY_U                  85
#define COMETA_KEY_V                  86
#define COMETA_KEY_W                  87
#define COMETA_KEY_X                  88
#define COMETA_KEY_Y                  89
#define COMETA_KEY_Z                  90
#define COMETA_KEY_LEFT_BRACKET       91  /* [ */
#define COMETA_KEY_BACKSLASH          92  /* \ */
#define COMETA_KEY_RIGHT_BRACKET      93  /* ] */
#define COMETA_KEY_GRAVE_ACCENT       96  /* ` */
#define COMETA_KEY_WORLD_1            161 /* non-US #1 */
#define COMETA_KEY_WORLD_2            162 /* non-US #2 */

// Complementary keys
//
#define COMETA_KEY_ESCAPE             256
#define COMETA_KEY_ENTER              257
#define COMETA_KEY_TAB                258
#define COMETA_KEY_BACKSPACE          259
#define COMETA_KEY_INSERT             260
#define COMETA_KEY_DELETE             261
#define COMETA_KEY_RIGHT              262
#define COMETA_KEY_LEFT               263
#define COMETA_KEY_DOWN               264
#define COMETA_KEY_UP                 265
#define COMETA_KEY_PAGE_UP            266
#define COMETA_KEY_PAGE_DOWN          267
#define COMETA_KEY_HOME               268
#define COMETA_KEY_END                269
#define COMETA_KEY_CAPS_LOCK          280
#define COMETA_KEY_SCROLL_LOCK        281
#define COMETA_KEY_NUM_LOCK           282
#define COMETA_KEY_PRINT_SCREEN       283
#define COMETA_KEY_PAUSE              284
#define COMETA_KEY_F1                 290
#define COMETA_KEY_F2                 291
#define COMETA_KEY_F3                 292
#define COMETA_KEY_F4                 293
#define COMETA_KEY_F5                 294
#define COMETA_KEY_F6                 295
#define COMETA_KEY_F7                 296
#define COMETA_KEY_F8                 297
#define COMETA_KEY_F9                 298
#define COMETA_KEY_F10                299
#define COMETA_KEY_F11                300
#define COMETA_KEY_F12                301
#define COMETA_KEY_F13                302
#define COMETA_KEY_F14                303
#define COMETA_KEY_F15                304
#define COMETA_KEY_F16                305
#define COMETA_KEY_F17                306
#define COMETA_KEY_F18                307
#define COMETA_KEY_F19                308
#define COMETA_KEY_F20                309
#define COMETA_KEY_F21                310
#define COMETA_KEY_F22                311
#define COMETA_KEY_F23                312
#define COMETA_KEY_F24                313
#define COMETA_KEY_F25                314
#define COMETA_KEY_KP_0               320
#define COMETA_KEY_KP_1               321
#define COMETA_KEY_KP_2               322
#define COMETA_KEY_KP_3               323
#define COMETA_KEY_KP_4               324
#define COMETA_KEY_KP_5               325
#define COMETA_KEY_KP_6               326
#define COMETA_KEY_KP_7               327
#define COMETA_KEY_KP_8               328
#define COMETA_KEY_KP_9               329
#define COMETA_KEY_KP_DECIMAL         330
#define COMETA_KEY_KP_DIVIDE          331
#define COMETA_KEY_KP_MULTIPLY        332
#define COMETA_KEY_KP_SUBTRACT        333
#define COMETA_KEY_KP_ADD             334
#define COMETA_KEY_KP_ENTER           335
#define COMETA_KEY_KP_EQUAL           336
#define COMETA_KEY_LEFT_SHIFT         340
#define COMETA_KEY_LEFT_CONTROL       341
#define COMETA_KEY_LEFT_ALT           342
#define COMETA_KEY_LEFT_SUPER         343
#define COMETA_KEY_RIGHT_SHIFT        344
#define COMETA_KEY_RIGHT_CONTROL      345
#define COMETA_KEY_RIGHT_ALT          346
#define COMETA_KEY_RIGHT_SUPER        347
#define COMETA_KEY_MENU               348

#define COMETA_KEY_LAST               COMETA_KEY_MENU


// If this bit is set, one of the Shift keys is pressed
//
#define COMETA_MOD_SHIFT           0x0001

// If this bit is set, one of the Ctrl keys is pressed
//
#define COMETA_MOD_CONTROL         0x0002

// If this bit is set, ALT is being pressed
//
#define COMETA_MOD_ALT             0x0004

// Super keys enabled
//
#define COMETA_MOD_SUPER           0x0008

// if this bit is sets. caps are locked on the keyboard
//
#define COMETA_MOD_CAPS_LOCK       0x0010

// if this bit is set, Nums are locked on the numeric pad
//
#define COMETA_MOD_NUM_LOCK        0x0020

// Mouse buttons input
//
#define COMETA_MOUSE_BUTTON_1         0
#define COMETA_MOUSE_BUTTON_2         1
#define COMETA_MOUSE_BUTTON_3         2
#define COMETA_MOUSE_BUTTON_4         3
#define COMETA_MOUSE_BUTTON_5         4
#define COMETA_MOUSE_BUTTON_6         5
#define COMETA_MOUSE_BUTTON_7         6
#define COMETA_MOUSE_BUTTON_8         7
#define COMETA_MOUSE_BUTTON_LAST      COMETA_MOUSE_BUTTON_8
#define COMETA_MOUSE_BUTTON_LEFT      COMETA_MOUSE_BUTTON_1
#define COMETA_MOUSE_BUTTON_RIGHT     COMETA_MOUSE_BUTTON_2
#define COMETA_MOUSE_BUTTON_MIDDLE    COMETA_MOUSE_BUTTON_3

// Gamepad input
//
#define COMETA_GAMEPAD_BUTTON_A               0
#define COMETA_GAMEPAD_BUTTON_B               1
#define COMETA_GAMEPAD_BUTTON_X               2
#define COMETA_GAMEPAD_BUTTON_Y               3
#define COMETA_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define COMETA_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define COMETA_GAMEPAD_BUTTON_BACK            6
#define COMETA_GAMEPAD_BUTTON_START           7
#define COMETA_GAMEPAD_BUTTON_GUIDE           8
#define COMETA_GAMEPAD_BUTTON_LEFT_THUMB      9
#define COMETA_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define COMETA_GAMEPAD_BUTTON_DPAD_UP         11
#define COMETA_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define COMETA_GAMEPAD_BUTTON_DPAD_DOWN       13
#define COMETA_GAMEPAD_BUTTON_DPAD_LEFT       14
#define COMETA_GAMEPAD_BUTTON_LAST            COMETA_GAMEPAD_BUTTON_DPAD_LEFT

#define COMETA_GAMEPAD_BUTTON_CROSS       COMETA_GAMEPAD_BUTTON_A
#define COMETA_GAMEPAD_BUTTON_CIRCLE      COMETA_GAMEPAD_BUTTON_B
#define COMETA_GAMEPAD_BUTTON_SQUARE      COMETA_GAMEPAD_BUTTON_X
#define COMETA_GAMEPAD_BUTTON_TRIANGLE    COMETA_GAMEPAD_BUTTON_Y

enum CometaGamepadAxis{
    LEFT_X =         0,
    LEFT_Y =         1,
    RIGHT_X =        2,
    RIGHT_Y =        3,
    LEFT_TRIGGER =   4,
    RIGHT_TRIGGER =  5,
    LAST =           5
};
#endif //COMETAKEYS_H
