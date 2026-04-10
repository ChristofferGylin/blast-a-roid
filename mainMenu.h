#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include <stdbool.h>

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
void initMenu(Menu* menu);
void mainMenu();
void updateMenu(Menu* menu);

#endif