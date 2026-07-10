#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

void drawCheckbox(Vector2 position, bool state);
bool updateCheckbox(Vector2 position, bool* state);

#endif