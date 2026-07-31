#ifndef UI_H
#define UI_H

#include <stdbool.h>

#include "raylib.h"
#include "utils.h"

#define TITLE_MAX_LENGTH 32
#define DROPDOWN_MAX_ITEMS 32
#define DROPDOWN_MAX_LENGTH 64

typedef void (*DropDownCallback)(int selected, void* userData);
typedef void (*ButtonCallback)(void* userData);
typedef void (*DrawSectionContent)(void* userData);

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
    Callback callback;
    void* userData;
}Checkbox;

typedef struct CheckboxWithTitle {
    Checkbox checkbox;
    Vector2 titlePosition;
    char title[32];
    Callback callback;
    void* userData;
}CheckboxWithTitle;

typedef char DropDownTitles[DROPDOWN_MAX_ITEMS][DROPDOWN_MAX_LENGTH];

typedef struct DropdownItem {
    Rectangle rect;
    char title[DROPDOWN_MAX_LENGTH];
    Vector2 titlePosition;
    bool isHovered;
}DropdownItem;

typedef struct DropdownMenu {
    DropdownItem items[DROPDOWN_MAX_ITEMS];
    int itemCount;
    int selected;
    bool isHovered;
    bool isOpen;
    Rectangle button;
    Rectangle downArrow;
    Rectangle rectClosed;
    Rectangle rectOpen;
    Rectangle textArea;
    Vector2 titlePosition;
    DropDownCallback callback;
    void* userData;
}DropdownMenu;


typedef struct LayoutSection {
    Rectangle container;
    Rectangle contentArea;
    Rectangle divider;
    Vector2 headingPosition;
    DrawSectionContent drawContent;
    void* userData;
    char heading[TITLE_MAX_LENGTH];
}LayoutSection;

typedef struct OnClickIncreaseArgs {
    int* value;
    int max_Value;
}OnClickIncreaseArgs;

void drawBasicLayoutContainer(BasicLayoutContainer* layout);
void drawButton(Button* button);
void drawCheckbox(Checkbox* checkbox);
void drawCheckboxWithTitle(CheckboxWithTitle* option);
void drawDownArrow(Vector2 position, float width, Color color);
void drawDropdownMenu(DropdownMenu* menu);
void drawLayoutSection(LayoutSection* section);
void initBasicLayoutContainer(BasicLayoutContainer* layout, Rectangle area, char* heading);
void initButton(Button* button, Rectangle rect, int fontSize, char* text, ButtonCallback callback, void* userData);
void initCheckbox(Checkbox* checkbox, bool* state, Vector2 position, Callback callback, void* userData);
void initCheckboxWithTitle(CheckboxWithTitle* option, Vector2 position, char* title, bool* state, Callback callback, void* userData);
void initDropdownMenu(DropdownMenu* menu, DropDownTitles items, int itemsCount, int selected, Rectangle rect, DropDownCallback callback, void* userData);
void initLayoutSection(LayoutSection* section, Rectangle* parent, Rectangle container, char* heading, DrawSectionContent drawContent, void* userData);
void onClickBack(void* userData);
void onClickDecrease(void* userData);
void onClickIncrease(void* userData);
void updateButton(Button* button);
bool updateCheckbox(Checkbox* checkbox);
bool updateDropdownMenu(DropdownMenu* menu);

#endif