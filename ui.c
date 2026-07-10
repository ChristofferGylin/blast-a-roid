#include <stdbool.h>

#include "colors.h"
#include "constants.h"
#include "raylib.h"
#include "utils.h"
#include "ui.h"

int fontSize = 32;
int fontSpacing = 6;
int nextItemGap = 20;
int underLineHeight = 3;
int underLineOffset = 3;
int margin = 20;
int roundnessRadius = 12.0f;
int logoFontSize = 42;
int logoFontSpacing = 8;
int lineThickness = 3;

static Rectangle drawLayoutContainers();

Rectangle drawLayoutContainers() {

    Rectangle background = {
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    Rectangle mainContainer = {
        margin,
        margin,
        SCREEN_WIDTH - (margin * 2),
        SCREEN_HEIGHT - (margin * 2)
    };

    char headingText[] = "DEBUG OPTIONS";

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), headingText, logoFontSize, logoFontSpacing);

    Vector2 headingPos = {
        (SCREEN_WIDTH / 2) - (headingSize.x / 2),
        mainContainer.y + margin
    };

    DrawRectangleGradientV(background.x, background.y, background.width, background.height, topColor, bottomColor);

    DrawTextPro(
        GetFontDefault(),
        headingText,
        headingPos,
        (Vector2){0, 0},
        0,
        logoFontSize,
        logoFontSpacing,
        primaryColor
    );

    Vector2 linePos = {
        mainContainer.x,
        headingPos.y + headingSize.y + 10
    };

    Vector2 lineSize = {
        mainContainer.width,
        lineThickness,
    };

    DrawRectangle(linePos.x, linePos.y, lineSize.x, lineSize.y, primaryColor);

    DrawRectangleRoundedLinesEx(mainContainer, getRoundness(mainContainer, roundnessRadius), 10, lineThickness, primaryColor);

    Rectangle contentContainer;
    contentContainer.x = mainContainer.x;
    contentContainer.y = linePos.y + lineSize.y;
    contentContainer.width = mainContainer.width;
    contentContainer.height = mainContainer.height - (contentContainer.y - (mainContainer.y + lineSize.y));

    return contentContainer;
}

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