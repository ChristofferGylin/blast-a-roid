#include "pauseMenu.h"
#include "raylib.h"
#include "constants.h"
#include <stdbool.h>

void drawPausMenuBackground() {

    int menuWidth = 400;
    int menuHeight = 500;

    Rectangle background = {
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    Rectangle menuContainer = {
        (SCREEN_WIDTH / 2) - (menuWidth / 2),
        (SCREEN_HEIGHT / 2) - (menuHeight / 2),
        menuWidth,
        menuHeight
    };

    Vector2 origin = {0, 0};

    Color backgroundColor = Fade(BLACK, 0.6f);
    Color menuColor = Fade(DARKBLUE, 0.6f);

    DrawRectanglePro(background, origin, 0, backgroundColor);
    DrawRectanglePro(menuContainer, origin, 0, menuColor);
}
