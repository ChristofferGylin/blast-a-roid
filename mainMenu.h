#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_NUMBER_OF_MAIN_MENU_ITEMS 5

typedef struct GameContext GameContext;

typedef struct HighlightTimers {
    float blinkTimer;
    float activeTimer;
    bool blinkVisible;
    bool active;
}HighlightTimers;

typedef struct MenuItem {
    char text[32];
    Vector2 basePosition;
    Vector2 size;
    
    bool isHovered;
}MenuItem;

typedef struct Menu {
    MenuItem items[MAX_NUMBER_OF_MAIN_MENU_ITEMS];
    int count;
    int selected;
    int menuOffset;
}Menu; 

Rectangle drawLayoutContainers();
void drawMenu(Menu* menu);
void mainMenu(GameContext* ctx);
void updateMenu(Menu* menu);

#endif