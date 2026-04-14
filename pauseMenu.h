#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "raylib.h"

typedef struct PausMenuItem {
    char text[32];
    Vector2 basePosition;
    Vector2 size;
    
    bool isHovered;
}PausMenuItem;

typedef struct PausMenuRects {
    Rectangle background;
    Rectangle menuContainer;
}PausMenuRects;

typedef struct PauseMenuColors {
    Color background;
    Color menu;
    Color text;
}PauseMenuColors;

typedef struct PausMenu {
    PausMenuItem items[4];
    int count;
    int selected;
    int menuOffset;
    PausMenuRects rects;
    PauseMenuColors colors;
}PausMenu; 

void drawPausMenu(PausMenu* menu);
void initPausMenu(PausMenu* menu);
void updatePausMenu(PausMenu* menu);

#endif