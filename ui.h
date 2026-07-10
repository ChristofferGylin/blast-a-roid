#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"

static const float CHECKBOX_SIZE = 22.0f;

Rectangle renderCheckbox(Vector2 position, bool isChecked);

#endif