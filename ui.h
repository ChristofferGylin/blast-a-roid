#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

typedef void (*ButtonCallback)(void* userData);

typedef struct Button {
    Rectangle rect;
    Vector2 textPosition;
    int fontSize;
    bool isHovered;
    ButtonCallback onClick;
    void* userData;
    char text[];
}Button;

void drawButton(Button* button);
void drawCheckbox(Vector2 position, bool state);
void initButton(Button* button, Rectangle rect, int fontSize, char* text, ButtonCallback callback, void* userData);
bool updateCheckbox(Vector2 position, bool* state);

#endif