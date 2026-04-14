#include "pauseMenu.h"
#include "raylib.h"
#include "constants.h"
#include <stdbool.h>
#include "mainMenu.h"

void drawPausMenuBackground(PausMenu* menu) {

    Vector2 origin = {0, 0};

    DrawRectanglePro(menu->rects.background, origin, 0, menu->colors.background);
    DrawRectanglePro(menu->rects.menuContainer, origin, 0, menu->colors.menu);
}

void initPausMenu(PausMenu* menu) {

    int menuWidth = 400;
    int menuHeight = 500;
    int fontSize = 32;
    int fontSpacing = 6;
    int nextItemGap = 20;
    
    char titles[][32] = {
        "RESUME GAME",
        "OPTIONS",
        "EXIT TO MENU",
        "EXIT TO DESKTOP"
    };

    int menuY = 0;

    menu->rects.background = (Rectangle){
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    menu->rects.menuContainer = (Rectangle){
        (SCREEN_WIDTH / 2) - (menuWidth / 2),
        (SCREEN_HEIGHT / 2) - (menuHeight / 2),
        menuWidth,
        menuHeight
    };

    menu->colors.background = (Color) Fade(BLACK, 0.6f);
    menu->colors.menu = (Color) Fade(DARKBLUE, 0.6f);
    menu->colors.text = (Color) RAYWHITE;

    for (int i = 0; i < 4; i++) {
        Vector2 size = MeasureTextEx(GetFontDefault(), titles[i], fontSize, fontSpacing);

        menu->items[i].basePosition = (Vector2){
            (SCREEN_WIDTH / 2) - (size.x / 2),
            menuY
        };

        strcpy(menu->items[i].text, titles[i]);
        menu->items[i].size = size;
        menu->items[i].isHovered = false;

        menuY += size.y + nextItemGap;
    }

    menu->count = 4;

    int menuHeight = menuY - nextItemGap;
    menu->menuOffset = (SCREEN_HEIGHT / 2) - (menuHeight / 2);

    menu->selected = -1;
}
