#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef struct VideoOptions {
    bool showFps;
}VideoOptions;

typedef struct Options {
    VideoOptions video;
};


#endif