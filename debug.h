#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

typedef struct Debug {
    bool active;
}Debug;

void initDebug(Debug* debug, bool active);

#endif