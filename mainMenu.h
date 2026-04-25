#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include <stdbool.h>

typedef struct GameContext GameContext;

typedef struct MenuItem {
    char text[32];
    Vector2 basePosition;
    Vector2 size;
    
    bool isHovered;
}MenuItem;

typedef struct Menu {
    MenuItem items[4];
    int count;
    int selected;
    int menuOffset;
}Menu; 

void drawLayoutContainers();
void drawMenu(Menu* menu);
float getRoundness(Rectangle rect, float radiusPx);
void initMenu(Menu* menu);
void mainMenu(GameContext* ctx);
void updateMenu(Menu* menu);

#endif