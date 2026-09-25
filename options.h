#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

#include "raylib.h"


static const bool FULLSCREEN_DEFAULT_VALUE = false;
static const bool IS_MONITOR_SET_BY_USER_DEFAULT_VALUE = false;
static const int SELECTED_MONITOR_DEFAULT_VALUE = 0;
static const bool SHOW_FPS_DEFAULT_VALUE = false;
static const bool IS_V_SYNC_ENABLED_DEFAULT_VALUE = false;

static const int KEY_BIND_LEFT_DEFAULT_VALUE = KEY_A;
static const int KEY_BIND_RIGHT_DEFAULT_VALUE = KEY_D;
static const int KEY_BIND_THRUST_DEFAULT_VALUE = KEY_W;
static const int KEY_BIND_FIRE_DEFAULT_VALUE = KEY_RIGHT_CONTROL;
static const int KEY_BIND_SHIELD_DEFAULT_VALUE = KEY_SPACE;

#define NUMBER_OF_CONTROLS 5

static const int availibleKeys[] = {
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_ZERO,
    KEY_ONE,
    KEY_TWO,
    KEY_THREE,
    KEY_FOUR,
    KEY_FIVE,
    KEY_SIX,
    KEY_SEVEN,
    KEY_EIGHT,
    KEY_NINE,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET,
    KEY_GRAVE,
    // Function keys
    KEY_SPACE,
    KEY_ESCAPE,
    KEY_ENTER,
    KEY_TAB,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_SCROLL_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_KB_MENU,
    // Keypad keys
    KEY_KP_0,
    KEY_KP_1,
    KEY_KP_2,
    KEY_KP_3,
    KEY_KP_4,
    KEY_KP_5,
    KEY_KP_6,
    KEY_KP_7,
    KEY_KP_8,
    KEY_KP_9,
    KEY_KP_DECIMAL,
    KEY_KP_DIVIDE,
    KEY_KP_MULTIPLY,
    KEY_KP_SUBTRACT,
    KEY_KP_ADD,
    KEY_KP_ENTER,
    KEY_KP_EQUAL,
};

typedef struct KeyBind {
    char name[32];
    int key;
}KeyBind;

typedef struct KeyBindings {
    KeyBind left;
    KeyBind right;
    KeyBind thrust;
    KeyBind fire;
    KeyBind shield;
}KeyBindings;

typedef struct ControlsOptions {
    KeyBindings keys;
}ControlsOptions;

typedef struct VideoOptions {
    bool fullscreen;
    bool showFps;
    bool vSync;
    bool isMonitorSetByUser;
    int selectecMonitor;
}VideoOptions;

typedef struct Options {
    ControlsOptions controls;
    VideoOptions video;
}Options;

void setUserRefreshRate();
void resetControlsToDefault(ControlsOptions* controls);
void resetOptionsToDefault(Options* options);
void resetVideoOptionsToDefault(VideoOptions* options);

#endif