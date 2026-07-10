#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

static const float CHECKBOX_SIZE = 22.0f;

typedef struct Checkbox {
    Vector2 position;
    bool isChecked;
}Checkbox;

void renderCheckbox(Checkbox* checkbox);
bool updateCheckbox(Checkbox* checkbox, void (*callback)(bool));

#endif