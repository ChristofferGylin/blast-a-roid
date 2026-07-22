#include <string.h>

#include "optionsMenu.h"
#include "raylib.h"
#include "ui.h"
#include "uiSizes.h"

void initOptionsMenuTab(LayoutSection* section, Rectangle* parent, char* heading, DrawSectionContent drawContent, void* userData);

void initOptionsMenuTab(LayoutSection* section, Rectangle* parent, char* heading, DrawSectionContent drawContent, void* userData) {

    section->drawContent = drawContent;
    section->userData = userData;

    section->container.x += parent->x + MENU_MARGIN;
    section->container.y += parent->y + MENU_MARGIN;
    section->container.width = parent->width;
    section->container.height = parent->height;

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), heading, SECTION_HEADING_FONT_SIZE, MENU_FONT_SPACING);

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