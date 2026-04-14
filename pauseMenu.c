#include "pauseMenu.h"
#include "raylib.h"
#include "constants.h"
#include <stdbool.h>
#include "mainMenu.h"

int menuWidth = 400;
int menuHeight = 500;
int fontSize = 32;
int fontSpacing = 6;
int nextItemGap = 20;
int underLineHeight = 3;
int underLineOffset = 3;

void drawPausMenu(PausMenu* menu) {
    Vector2 origin = {0, 0};

    DrawRectanglePro(menu->rects.background, origin, 0, menu->colors.background);
    DrawRectanglePro(menu->rects.menuContainer, origin, 0, menu->colors.menu);

    for (int i = 0; i < menu->count; i++) {
        PausMenuItem* item = &menu->items[i];

        Vector2 pos = {
            item->basePosition.x,
            item->basePosition.y + menu->menuOffset
        };

        DrawTextPro(
            GetFontDefault(),
            item->text,
            pos,
            origin,
            0,
            fontSize,
            fontSpacing,
            menu->colors.text
        );

        if (item->isHovered) {
            DrawRectangle(
                pos.x,
                pos.y + item->size.y + underLineOffset,
                item->size.x,
                underLineHeight,
                menu->colors.text
            );
        }
    }
}

void initPausMenu(PausMenu* menu) {    
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
