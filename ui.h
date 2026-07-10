#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

static const float CHECKBOX_SIZE = 22.0f;

typedef struct Checkbox {
    bool isChecked;
    Vector2 position;
}Checkbox;

void renderCheckbox(Checkbox* checkbox);

#endif