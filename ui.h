#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

typedef void (*ButtonCallback)(void* userData);

typedef struct Button {
    Rectangle rect;
    int fontSize;
    ButtonCallback onClick;
    void* userData;
    char text[];
}Button;

void drawCheckbox(Vector2 position, bool state);
bool updateCheckbox(Vector2 position, bool* state);

#endif