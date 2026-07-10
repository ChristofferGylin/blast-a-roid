#include <stdbool.h>
#include <string.h>

#include "colors.h"
#include "constants.h"
#include "raylib.h"
#include "utils.h"
#include "ui.h"
#include "uiSizes.h"

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

void drawButton(Button* button) {
    Color buttonColor = button->isHovered ? primaryColorDimmed50 : primaryColorDimmed30;
    float roundness = getRoundness(button->rect, BUTTON_ROUNDNESS_RADIUS);

    DrawRectangleRounded(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, BLACK);
    DrawRectangleRounded(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, buttonColor);
    DrawRectangleRoundedLinesEx(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, 2, primaryColor);

    DrawTextPro(GetFontDefault(), button->text, button->textPosition, (Vector2){0,0}, 0, button->fontSize, BUTTON_FONT_SPACING, primaryColor);
}

void initButton(Button* button, Rectangle rect, int fontSize, char* text, ButtonCallback callback, void* userData) {
    button->fontSize = fontSize;
    button->onClick = callback;
    button->rect = rect;
    button->userData = userData;
    strncpy(button->text, text, sizeof(button->text) - 1);
    button->text[sizeof(button->text) - 1] = '\0';

    Vector2 textSize = MeasureTextEx(GetFontDefault(), button->text, button->fontSize, BUTTON_FONT_SPACING);

    if (textSize.x + (BUTTON_PADDING * 2) > button->rect.width) {
        button->rect.width = textSize.x + (BUTTON_PADDING * 2);
    } 

    if (textSize.y + (BUTTON_PADDING * 2) > button->rect.height) {
        button->rect.height = textSize.y + (BUTTON_PADDING * 2);
    }

    button->textPosition.x = button->rect.x + (button->rect.width / 2.0f) - (textSize.x / 2.0f);
    button->textPosition.y = button->rect.y + (button->rect.height / 2.0f) - (textSize.y / 2.0f);
}

void updateButton(Button* button) {
    if (CheckCollisionPointRec(GetMousePosition(), button->rect)) {
        button->isHovered = true;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            button->onClick(button->userData);
        }

    } else {
        button->isHovered = false;
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