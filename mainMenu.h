#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"
#include <stdbool.h>

typedef struct MenuItem {
    Vector2 position;
    Vector2 height;
    char text[32];
    bool isUnderlined;
}MenuItem;

void mainMenu();
MenuItem generateMenuItem(int number, char title[], int* startY);

#endif