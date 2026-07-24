#include <stdio.h>

#include "showFps.h"
#include "raylib.h"

int main() {

    InitWindow(800, 600, "Test");

    int finalSize = 0;

    int fontSize = 12;

    while(finalSize == 0) {
        Vector2 titleSize = MeasureTextEx(GetFontDefault(), "CURRENT", FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
        Vector2 valueSize = MeasureTextEx(GetFontDefault(), "FRAME", fontSize, FPS_TITLE_FONT_SPACING);

        if (valueSize.x > titleSize.x) {
            finalSize = fontSize - 1;
        } else {
            fontSize++;
        }
    }

    printf("\nFinal font size: %d\n", finalSize);

    CloseWindow();

    return 0;
}