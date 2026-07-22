#include <string.h>

#include "colors.h"
#include "constants.h"
#include "optionsMenu.h"
#include "raylib.h"
#include "ui.h"
#include "uiSizes.h"

void drawOptionsMenu(OptionsMenu* menu);
void drawOptionsMenuTab(OptionsMenu* menu);
void initOptionsMenu(OptionsMenu* menu);
void initOptionsMenuTab(LayoutSection* section, Rectangle* parent, char* heading, DrawSectionContent drawContent, void* userData);
void drawVideoTab(void* userData);
void drawControlsTab(void* userData);
void drawAudioTab(void* userData);


void drawVideoTab(void* userData) {};
void drawControlsTab(void* userData) {};
void drawAudioTab(void* userData) {};

void initOptionsMenu(OptionsMenu* menu) {
    menu->exit = false;
    menu->onClickIncreaseArgs.max_Value = NUMBER_OF_OPTIONS_TABS;
    menu->onClickIncreaseArgs.value = &menu->selectecTab;

    initBasicLayoutContainer(&menu->layout, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, "OPTIONS");
    initButton(
        &menu->backButton,
        (Rectangle){MENU_MARGIN * 2.0f, (MENU_MARGIN * 2.0f) + MENU_LINE_THICKNESS, 0, 0},
        BUTTON_FONT_SIZE, "BACK",
        onClickBack,
        &menu->exit
    );
    
    initOptionsMenuTab(&menu->tabs[0], &menu->layout.contentArea, "VIDEO", drawVideoTab, &menu->videoTabData);
    initOptionsMenuTab(&menu->tabs[1], &menu->layout.contentArea, "AUDIO", drawAudioTab, &menu->audioTabData);
    initOptionsMenuTab(&menu->tabs[2], &menu->layout.contentArea, "CONTROLS", drawControlsTab, &menu->controlsTabData);

    float largestHeadingSize = 0;

    for (int i = 0; i < NUMBER_OF_OPTIONS_TABS; i++) {
        Vector2 headingSize = MeasureTextEx(GetFontDefault(), menu->tabs[i].heading, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING);

        if (headingSize.x > largestHeadingSize) largestHeadingSize = headingSize.x;
    }

    Rectangle previousTabButtonRect;

    previousTabButtonRect.width = 32;
    previousTabButtonRect.height = 32;
    previousTabButtonRect.x = menu->tabs[0].contentArea.x + (menu->tabs[0].contentArea.width / 2.0f) - (largestHeadingSize / 2.0f) - (previousTabButtonRect.width / 2.0f);
    previousTabButtonRect.y = menu->tabs[0].headingPosition.y;

    Rectangle nextTabButtonRect;

    previousTabButtonRect.width = 32;
    previousTabButtonRect.height = 32;
    previousTabButtonRect.x = menu->tabs[0].contentArea.x + (menu->tabs[0].contentArea.width / 2.0f) + (largestHeadingSize / 2.0f) + (nextTabButtonRect.width / 2.0f);
    previousTabButtonRect.y = menu->tabs[0].headingPosition.y;

    initButton(&menu->nextTabButton, nextTabButtonRect, OPTIONS_TAB_HEADING_FONT_SIZE, ">", onClickIncrease, &menu->onClickIncreaseArgs);
    initButton(&menu->prevTabButton, previousTabButtonRect, OPTIONS_TAB_HEADING_FONT_SIZE, "<", onClickDecrease, &menu->onClickIncreaseArgs);

}

void initOptionsMenuTab(LayoutSection* section, Rectangle* parent, char* heading, DrawSectionContent drawContent, void* userData) {

    section->drawContent = drawContent;
    section->userData = userData;

    section->container.x += parent->x + MENU_MARGIN;
    section->container.y += parent->y + MENU_MARGIN;
    section->container.width = parent->width;
    section->container.height = parent->height;

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), heading, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING);

    section->headingPosition.x = section->container.x + (section->container.width / 2.0f) + (headingSize.x / 2.0f);
    section->headingPosition.y = section->container.y;

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

void drawOptionsMenu(OptionsMenu* menu) {
    BeginDrawing();
        drawBasicLayoutContainer(&menu->layout);
        drawButton(&menu->backButton);
        drawOptionsMenuTab(menu);
    EndDrawing();
}

void drawOptionsMenuTab(OptionsMenu* menu) {

    LayoutSection* section = &menu->tabs[menu->selectecTab];

    Vector2 origin = {0,0};

    drawButton(&menu->nextTabButton);
    drawButton(&menu->prevTabButton);
    DrawTextPro(GetFontDefault(), section->heading, section->headingPosition, origin, 0, OPTIONS_TAB_HEADING_FONT_SIZE, MENU_FONT_SPACING, primaryColor);
    DrawRectanglePro(section->divider, origin, 0, primaryColor);

    section->drawContent(section->userData);
}