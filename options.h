#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>


static const bool FULLSCREEN_DEFAULT_VALUE = false;
static const bool IS_MONITOR_SET_BY_USER_DEFAULT_VALUE = false;
static const int SELECTED_MONITOR_DEFAULT_VALUE = 0;
static const bool SHOW_FPS_DEFAULT_VALUE = false;
static const bool IS_V_SYNC_ENABLED_DEFAULT_VALUE = false;

typedef struct VideoOptions {
    bool fullscreen;
    bool showFps;
    bool vSync;
    bool isMonitorSetByUser;
    int selectecMonitor;
}VideoOptions;

typedef struct Options {
    VideoOptions video;
}Options;

void setUserRefreshRate();
void resetOptionsToDefault(Options* options);

#endif