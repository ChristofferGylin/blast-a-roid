#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"
#define TITLE_MAX_LENGTH 32

typedef void (*ButtonCallback)(void* userData);

typedef struct BasicLayoutContainer {
    Rectangle background;
    Rectangle container;
    Rectangle divider;
    Rectangle contentArea;
    Vector2 headingPosition;
    char heading[TITLE_MAX_LENGTH];
}BasicLayoutContainer;


typedef struct Button {
    Rectangle rect;
    Vector2 textPosition;
    int fontSize;
    bool isHovered;
    ButtonCallback onClick;
    void* userData;
    char text[TITLE_MAX_LENGTH];
}Button;

typedef struct Checkbox {
    Vector2 position;
    bool* state;
}Checkbox;

void drawBasicLayoutContainer(BasicLayoutContainer* layout);
void drawButton(Button* button);
void drawCheckbox(Checkbox* checkbox);
void initBasicLayoutContainer(BasicLayoutContainer* layout, Rectangle area, char* heading);
void initButton(Button* button, Rectangle rect, int fontSize, char* text, ButtonCallback callback, void* userData);
void initCheckbox(Checkbox* checkbox, bool* state, Vector2 position);
void updateButton(Button* button);
bool updateCheckbox(Vector2 position, bool* state);

#endif