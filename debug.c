#include <stdbool.h>

#include "debug.h"

void initDebug(Debug* debug, bool active) {
    debug->active = active;
}