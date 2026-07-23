#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

static const bool SHOW_FPS_DEFAULT_VALUE = false;

typedef struct VideoOptions {
    bool showFps;
}VideoOptions;

typedef struct Options {
    VideoOptions video;
}Options;

void resetOptionsToDefault(Options* options);

#endif