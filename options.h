#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef struct VideoOptions {
    bool showFps;
}VideoOptions;

typedef struct Options {
    VideoOptions video;
}Options;

void resetOptionsToDefault(Options* options);

#endif