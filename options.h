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

typedef struct KeyBindings {
    int left;
    int right;
    int thrust;
    int fire;
    int shield;
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