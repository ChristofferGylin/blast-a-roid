#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

static const bool SHOW_FPS_DEFAULT_VALUE = false;
static const bool IS_V_SYNC_ENABLED_DEFAULT_VALUE = false;

typedef struct VideoOptions {
    bool showFps;
    bool vSync;
}VideoOptions;

typedef struct Options {
    VideoOptions video;
}Options;

void setUserRefreshRate();
void resetOptionsToDefault(Options* options);

#endif