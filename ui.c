#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "colors.h"
#include "constants.h"
#include "raylib.h"
#include "utils.h"
#include "ui.h"
#include "uiSizes.h"

static const int CHECKBOX_FONT_SIZE = 18;

void drawCheckbox(Checkbox* checkbox) {
    
    const float lineThickness = 3.0f;
    const float roundness = 0.5f;
    const int segments = 10;

    Rectangle box = {checkbox->position.x, checkbox->position.y, CHECKBOX_SIZE, CHECKBOX_SIZE};
    
    DrawRectangleRounded(box, roundness, segments, primaryColorDimmed30);
    DrawRectangleRoundedLinesEx(box, 0.5f, 10, lineThickness, primaryColor);

    if (*checkbox->state) {
        float inset = 3.0f;

        Vector2 p1 = {
            box.x + inset,
            box.y + box.height * 0.55f
        };

        Vector2 p2 = {
            box.x + box.width * 0.42f,
            box.y + box.height - inset
        };

        Vector2 p3 = {
            box.x + box.width - inset,
            box.y + inset
        };

        DrawLineEx(p1, p2, lineThickness, primaryColor);
        DrawLineEx(p2, p3, lineThickness, primaryColor);
    }
}


void drawCheckboxWithTitle(CheckboxWithTitle* option) {    
    drawCheckbox(&option->checkbox);
    DrawTextPro(GetFontDefault(), option->title, option->titlePosition, (Vector2){0,0}, 0, CHECKBOX_FONT_SIZE, MENU_FONT_SPACING, primaryColor);
}

void drawBasicLayoutContainer(BasicLayoutContainer* layout) {
    DrawRectangleGradientV(layout->background.x, layout->background.y, layout->background.width, layout->background.height, topColor, bottomColor);

    DrawTextPro(
        GetFontDefault(),
        layout->heading,
        layout->headingPosition,
        (Vector2){0, 0},
        0,
        MENU_HEADING_FONT_SIZE,
        MENU_HEADING_FONT_SPACING,
        primaryColor
    );

    DrawRectanglePro(layout->divider, (Vector2){0,0}, 0, primaryColor);
    DrawRectangleRoundedLinesEx(layout->container, getRoundness(layout->container, MENU_ROUNDNESS_RADIUS), MENU_ROUNDNESS_SEGMENTS, MENU_LINE_THICKNESS, primaryColor);
}

void drawButton(Button* button) {
    Color buttonColor = button->isHovered ? primaryColorDimmed50 : primaryColorDimmed30;
    float roundness = getRoundness(button->rect, BUTTON_ROUNDNESS_RADIUS);

    DrawRectangleRounded(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, BLACK);
    DrawRectangleRounded(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, buttonColor);
    DrawRectangleRoundedLinesEx(button->rect, roundness, MENU_ROUNDNESS_SEGMENTS, 2, primaryColor);

    DrawTextPro(GetFontDefault(), button->text, button->textPosition, (Vector2){0,0}, 0, button->fontSize, BUTTON_FONT_SPACING, primaryColor);
}

void drawLayoutSection(LayoutSection* section) {
    Vector2 origin = {0,0};
    
    DrawTextPro(GetFontDefault(), section->heading, section->headingPosition, origin, 0, SECTION_HEADING_FONT_SIZE, MENU_FONT_SPACING, primaryColor);
    DrawRectanglePro(section->divider, origin, 0, primaryColor);

    section->drawContent(section->userData);
}

void initBasicLayoutContainer(BasicLayoutContainer* layout, Rectangle area, char* heading) {
    layout->background = area;
    strcpy(layout->heading, heading);

    layout->container.x = area.x + MENU_MARGIN;
    layout->container.y = area.y + MENU_MARGIN;
    layout->container.width = area.width - (MENU_MARGIN * 2);
    layout->container.height = area.height - (MENU_MARGIN * 2);

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), layout->heading, MENU_HEADING_FONT_SIZE, MENU_HEADING_FONT_SPACING);

    layout->headingPosition.x = area.x + (area.width / 2.0f) - (headingSize.x / 2.0f);
    layout->headingPosition.y = layout->container.y + MENU_MARGIN + MENU_LINE_THICKNESS;

    layout->divider.x = layout->container.x;
    layout->divider.y = layout->headingPosition.y + headingSize.y + MENU_MARGIN;
    layout->divider.width = layout->container.width;
    layout->divider.height = MENU_LINE_THICKNESS;

    layout->contentArea.x = layout->container.x;
    layout->contentArea.y = layout->divider.y + layout->divider.height;
    layout->contentArea.width = layout->container.width;
    layout->contentArea.height = layout->container.height - (layout->contentArea.y - (layout->container.y + layout->divider.height));
}

void initButton(Button* button, Rectangle rect, int fontSize, char* text, ButtonCallback callback, void* userData) {
    button->fontSize = fontSize;
    button->onClick = callback;
    button->rect = rect;
    button->userData = userData;
    strncpy(button->text, text, sizeof(button->text) - 1);
    button->text[sizeof(button->text) - 1] = '\0';

    Vector2 textSize = MeasureTextEx(GetFontDefault(), button->text, button->fontSize, BUTTON_FONT_SPACING);

    if (textSize.x + (BUTTON_PADDING * 2) > button->rect.width) {
        button->rect.width = textSize.x + (BUTTON_PADDING * 2);
    } 

    if (textSize.y + (BUTTON_PADDING * 2) > button->rect.height) {
        button->rect.height = textSize.y + (BUTTON_PADDING * 2);
    }

    button->textPosition.x = button->rect.x + (button->rect.width / 2.0f) - (textSize.x / 2.0f);
    button->textPosition.y = button->rect.y + (button->rect.height / 2.0f) - (textSize.y / 2.0f);
}

void initCheckbox(Checkbox* checkbox, bool* state, Vector2 position) {
    checkbox->position = position;
    checkbox->state = state;
}

void initCheckboxWithTitle(CheckboxWithTitle* option, Vector2 position, char* title, bool* state) {
    
    int yCenter = position.y + ((MENU_MARGIN + CHECKBOX_SIZE) / 2.0f);
    
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, CHECKBOX_FONT_SIZE, MENU_FONT_SPACING);
    
    Vector2 checkBoxPosition = {position.x + MENU_MARGIN, yCenter - CHECKBOX_SIZE / 2.0f};
    Vector2 titlePosition = {
        checkBoxPosition.x + CHECKBOX_SIZE + (MENU_MARGIN / 2.0f),
        yCenter - (titleSize.y / 2.0f)
    };
    
    initCheckbox(&option->checkbox, state, checkBoxPosition);
    strcpy(option->title, title);
    option->titlePosition = titlePosition;
}

void initLayoutSection(LayoutSection* section, Rectangle* parent, Rectangle container, char* heading, DrawSectionContent drawContent, void* userData) {

    section->drawContent = drawContent;
    section->userData = userData;

    section->container = container;
    section->container.x += parent->x + MENU_MARGIN;
    section->container.y += parent->y + MENU_MARGIN;

    section->headingPosition.x = section->container.x;
    section->headingPosition.y = section->container.y;

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), heading, SECTION_HEADING_FONT_SIZE, MENU_FONT_SPACING);

    section->divider.x = section->headingPosition.x;
    section->divider.y = section->headingPosition.y + headingSize.y + (MENU_MARGIN / 2.0f); 
    section->divider.width = headingSize.x;
    section->divider.height = SECTION_DIVIDER_LINE_THICKNESS;

    section->contentArea.x = section->container.x;
    section->contentArea.y = section->divider.y + section->divider.height + MENU_MARGIN;
    section->contentArea.width = section->container.width;
    section->contentArea.height = section->container.y - section->contentArea.y;

    strcpy(section->heading, heading);
}

void updateButton(Button* button) {
    if (CheckCollisionPointRec(GetMousePosition(), button->rect)) {
        button->isHovered = true;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            button->onClick(button->userData);
        }

    } else {
        button->isHovered = false;
    }
}

bool updateCheckbox(Checkbox* checkbox) {

    bool isHovered = false;

    Rectangle box = {checkbox->position.x, checkbox->position.y, CHECKBOX_SIZE, CHECKBOX_SIZE};

    if (CheckCollisionPointRec(GetMousePosition(), box)) {
        isHovered = true;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            *checkbox->state = !*checkbox->state;
        }
    }

    return isHovered;
}