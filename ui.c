#include <stdbool.h>

#include "colors.h"
#include "raylib.h"
#include "ui.h"

void drawCheckbox(Vector2 position, bool state) {
    
    const float lineThickness = 3.0f;
    const float roundness = 0.5f;
    const int segments = 10;

    Rectangle box = {position.x, position.y, CHECKBOX_SIZE, CHECKBOX_SIZE};
    
    DrawRectangleRounded(box, roundness, segments, primaryColorDimmed30);
    DrawRectangleRoundedLinesEx(box, 0.5f, 10, lineThickness, primaryColor);

    if (state) {
        float inset = 3.0f;

        Vector2 p1 = {
            box.x + inset,
            box.y + box.height * 0.55f
        };

        Vector2 p2 = {
            box.x + box.width * 0.42f,
            box.y + box.height - inset
        };

        Vector2 p3 = {
            box.x + box.width - inset,
            box.y + inset
        };

        DrawLineEx(p1, p2, lineThickness, primaryColor);
        DrawLineEx(p2, p3, lineThickness, primaryColor);
    }
}

bool updateCheckbox(Vector2 position, bool* state) {
    bool isHovered = false;

    Rectangle box = {position.x, position.y, CHECKBOX_SIZE, CHECKBOX_SIZE};

    if (CheckCollisionPointRec(GetMousePosition(), box)) {
        isHovered = true;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *state = !*state;
        }
    }

    return isHovered;
}