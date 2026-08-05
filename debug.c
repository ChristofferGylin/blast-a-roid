#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "config.h"
#include "debug.h"
#include "gameContext.h"
#include "raylib.h"
#include "ui.h"
#include "uiSizes.h"

void drawPoolCountOption(ObjectCount* option, char* title, Vector2 position);
void drawObjectCountSection(ObjectCountSection* section);
void drawObjectCountSectionContent(void* userData);
void drawOutputOptions(void* userData);
void initDebugMenu(GameContext* ctx, DebugMenu* menu);
void initDebugOutputOptionsSection(GameContext* ctx, DebugOutputOptionsSection* section, Rectangle* parent);
void initObjectCountSection(GameContext* ctx, ObjectCountSection* section, Rectangle* parent);
void outputDebugToTerminal(Debug* debug);
void outputObjectCountToTerminal(const char* name, ObjectCount oc);
void resetObjectCount(ObjectCount* oc, int capacity);
bool updateObjectCountSection(ObjectCountSection* section);
bool updateOutputOptionsSection(DebugOutputOptionsSection* section);

bool debugMenu(GameContext* ctx) {

    Debug* debug = &ctx->debug;
    DebugMenu menu;

    Config initialConfigState = getConfig(ctx);

    initDebugMenu(ctx, &menu);

    bool applicationIsRunning = true;

    while (!WindowShouldClose()) {
        updateButton(&menu.backButton);
        bool isObjectCountCheckboxHovered = updateObjectCountSection(&menu.objectCountSection);
        bool isOutputOptionsCheckboxHovered = updateOutputOptionsSection(&menu.outputOptionsSection);

        if (
            isObjectCountCheckboxHovered ||
            isOutputOptionsCheckboxHovered ||
            menu.backButton.isHovered
        ) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        BeginDrawing();
            drawBasicLayoutContainer(&menu.layout);
            drawButton(&menu.backButton);
            drawLayoutSection(&menu.objectCountSection.section);
            drawLayoutSection(&menu.outputOptionsSection.section);
        EndDrawing();

        if (menu.exit) break;
    }

    if (WindowShouldClose()) {
        applicationIsRunning = false;
    } else {
        Config endConfigState = getConfig(ctx);
        
        if (!compareConfig(&initialConfigState, &endConfigState)) {
            saveConfigToFile(ctx);
        }
    }

    return applicationIsRunning;
}

void drawObjectCountSection(ObjectCountSection* section) {
    
    Vector2 origin = {0,0};
    
    DrawTextPro(GetFontDefault(), section->section.heading, section->section.headingPosition, origin, 0, SECTION_HEADING_FONT_SIZE, MENU_FONT_SPACING, primaryColor);
    DrawRectanglePro(section->section.divider, origin, 0, primaryColor);

    section->section.drawContent(&section->options);
}

void drawObjectCountSectionContent(void* userData) {
    CheckboxWithTitle* options = (CheckboxWithTitle*)userData;

    for (int i = 0; i < NUMBER_OF_POOL_COUNTS; i++) {
        drawCheckboxWithTitle(&options[i]);
    }
}

void drawOutputOptions(void* userData) {

    DebugOutputOptions* outputOptions = (DebugOutputOptions*)userData;

    DrawTextPro(
        GetFontDefault(),
        outputOptions->frequencyHeadingText,
        outputOptions->frequencyHeadingPosition,
        (Vector2){0, 0},
        0,
        SUB_SECTION_HEADING_FONT_SIZE,
        MENU_FONT_SPACING,
        primaryColor
    );

    drawCheckboxWithTitle(&outputOptions->outputOnChangeCheckbox);
    drawButton(&outputOptions->decreaseButton);
    drawButton(&outputOptions->increaseButton);

    float valueDisplayRoundness = getRoundness(outputOptions->valueDisplay, 8.0f);
    float valueDisplaySegments = 10;
    int valueFontSize = 22;
    int valueFontSpacing = 4;

    Rectangle* valueRect = &outputOptions->valueDisplay;

    char valueStr[32];

    snprintf(valueStr, sizeof(valueStr), "%.2f", *outputOptions->outputFrequency);

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueStr, valueFontSize, valueFontSpacing);
    Vector2 valuePosition;

    valuePosition.x = valueRect->x + (valueRect->width / 2.0f) - (valueSize.x / 2.0f);
    valuePosition.y = valueRect->y + (valueRect->height / 2.0f) - (valueSize.y / 2.0f);

    Vector2 origin = {0,0};

    DrawRectangleRounded(*valueRect, valueDisplayRoundness, valueDisplaySegments, primaryColorDimmed30);
    DrawRectangleRoundedLinesEx(*valueRect, valueDisplayRoundness, valueDisplaySegments, 2, primaryColor);
    DrawTextPro(GetFontDefault(), valueStr, valuePosition, origin, 0, valueFontSize, valueFontSpacing, primaryColor);
}

void initDebug(Debug* debug, bool active) {
    debug->active = active;
    
    debug->onlyOutputOnChange = true;
    debug->outputFrequency = 1.0f;

    debug->poolCount.asteroids.showInDebug = true;
    debug->poolCount.bonuses.showInDebug = true;
    debug->poolCount.destroyedAsteroids.showInDebug = true;
    debug->poolCount.enemies.showInDebug = true;
    debug->poolCount.explosions.showInDebug = true;
    debug->poolCount.shots.showInDebug = true;
    debug->poolCount.spawnableBonuses.showInDebug = true;
    debug->poolCount.spawnableEnemies.showInDebug = true;
    debug->poolCount.specials.showInDebug = true;
    debug->poolCount.specialsSpawn.showInDebug = true;

    resetDebug(debug);
}

void resetDebug(Debug* debug) {

    debug->updateTimer = 0.0f;

    resetObjectCount(&debug->poolCount.asteroids, MAX_ASTEROIDS);
    resetObjectCount(&debug->poolCount.bonuses, MAX_BONUSES);
    resetObjectCount(&debug->poolCount.destroyedAsteroids, MAX_ASTEROIDS);
    resetObjectCount(&debug->poolCount.enemies, MAX_ENEMIES);
    resetObjectCount(&debug->poolCount.explosions, MAX_ANIMATIONS);
    resetObjectCount(&debug->poolCount.shots, MAX_SHOTS);
    resetObjectCount(&debug->poolCount.spawnableBonuses, NUMBER_OF_BONUS_TYPES);
    resetObjectCount(&debug->poolCount.spawnableEnemies, NUMBER_OF_ENEMY_TYPES);
    resetObjectCount(&debug->poolCount.specials, NUMBER_OF_SPECIALS);
    resetObjectCount(&debug->poolCount.specialsSpawn, NUMBER_OF_SPECIALS);
}

void initDebugMenu(GameContext* ctx, DebugMenu* menu) {
    
    menu->exit = false;
    initBasicLayoutContainer(&menu->layout, (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, "DEBUG MENU");
    initButton(
        &menu->backButton,
        (Rectangle){MENU_MARGIN * 2.0f, (MENU_MARGIN * 2.0f) + MENU_LINE_THICKNESS, 0, 0},
        BUTTON_FONT_SIZE, "BACK",
        onClickBack,
        &menu->exit
    );
    initObjectCountSection(ctx, &menu->objectCountSection, &menu->layout.contentArea);
    initDebugOutputOptionsSection(ctx, &menu->outputOptionsSection, &menu->layout.contentArea);
}

void initObjectCountSection(GameContext* ctx, ObjectCountSection* section, Rectangle* parent) {
    int index = 0;
    int yPosition = 0;

    #define OUTPUT(name)                                                                                                        \
        do {                                                                                                                    \
            Vector2 optionPosition = {0, yPosition};                                                                            \
            initCheckboxWithTitle(&section->options[index], optionPosition, #name, &ctx->debug.poolCount.name.showInDebug, NULL, NULL);     \
            yPosition += MENU_MARGIN + CHECKBOX_SIZE;                                                                           \
            index++;                                                                                                            \
        } while (0);                                                                                                
    
    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    Rectangle layoutContainer;
    layoutContainer.x = 0;
    layoutContainer.y = 0;
    layoutContainer.width = parent->width / 2.0f;
    layoutContainer.height = parent->height / 2.0f;

    initLayoutSection(&section->section, parent, layoutContainer, "OBJECT COUNT OUTPUT", drawObjectCountSectionContent, &section->options); 

    for (int i = 0; i < NUMBER_OF_POOL_COUNTS; i++) {
        section->options[i].checkbox.position.x += section->section.contentArea.x;
        section->options[i].checkbox.position.y += section->section.contentArea.y;

        section->options[i].titlePosition.x += section->section.contentArea.x;
        section->options[i].titlePosition.y += section->section.contentArea.y;
    }
}

void initDebugOutputOptionsSection(GameContext* ctx, DebugOutputOptionsSection* section, Rectangle* parent) {

    section->options.decreaseButton = (Button){0};
    section->options.increaseButton = (Button){0};
    section->options.valueDisplay = (Rectangle){0};
    section->options.outputOnChangeCheckbox = (CheckboxWithTitle){0};
     
    section->options.onlyOutputOnChange = &ctx->debug.onlyOutputOnChange;
    section->options.outputFrequency = &ctx->debug.outputFrequency;

    section->options.frequencyHeadingPosition = (Vector2){0};
    strcpy(section->options.frequencyHeadingText, "FREQUENCY");

    Rectangle layoutContainer;
    layoutContainer.x = parent->width / 2.0f;
    layoutContainer.y = 0;
    layoutContainer.width = parent->width / 2.0f;
    layoutContainer.height = parent->height / 2.0f;

    initLayoutSection(&section->section, parent, layoutContainer, "OUTPUT OPTIONS", drawOutputOptions, &section->options);

    const float SIZE = 42.0f;
    const float GAP = 10.0f;
    const 

    Rectangle* contentArea = &section->section.contentArea;

    Rectangle decreaseButtonRect;
    Rectangle increaseButtonRect;
    Rectangle valueRect;

    Vector2 frequencyHeadingSize = MeasureTextEx(GetFontDefault(), section->options.frequencyHeadingText, SUB_SECTION_HEADING_FONT_SIZE, MENU_FONT_SPACING);

    decreaseButtonRect.width = SIZE;
    decreaseButtonRect.height = SIZE;

    increaseButtonRect.width = SIZE;
    increaseButtonRect.height = SIZE;

    valueRect.width = SIZE * 2.0f;
    valueRect.height = SIZE;

    section->options.frequencyHeadingPosition.x = contentArea->x;
    section->options.frequencyHeadingPosition.y = contentArea->y;

    decreaseButtonRect.x = contentArea->x;
    decreaseButtonRect.y = section->options.frequencyHeadingPosition.y + frequencyHeadingSize.y + GAP;

    valueRect.x = decreaseButtonRect.x + decreaseButtonRect.width + GAP;
    valueRect.y = decreaseButtonRect.y;

    increaseButtonRect.x = valueRect.x + valueRect.width + GAP;
    increaseButtonRect.y = decreaseButtonRect.y;

    section->options.valueDisplay = valueRect;

    Vector2 checkboxPosition;

    checkboxPosition.x = contentArea->x;
    checkboxPosition.y = decreaseButtonRect.y + SIZE + GAP;

    initCheckboxWithTitle(&section->options.outputOnChangeCheckbox, checkboxPosition, "Only output on change", &ctx->debug.onlyOutputOnChange, NULL, NULL);
    initButton(&section->options.decreaseButton, decreaseButtonRect, BUTTON_FONT_SIZE, "-", onClickDecrease, section->options.outputFrequency);
    initButton(&section->options.increaseButton, increaseButtonRect, BUTTON_FONT_SIZE, "+", onClickIncrease, section->options.outputFrequency);
}

void outputObjectCountToTerminal(const char* name, ObjectCount oc) {
    printf("%-21s Active count: %4d / %4d   Spike: %4d\n", name, oc.activeCount, oc.capacity, oc.spike);
}

void outputDebugToTerminal(Debug* debug) {

    #define OUTPUT(name)                                                \
        if (debug->poolCount.name.showInDebug) {                        \
            outputObjectCountToTerminal(#name, debug->poolCount.name);  \
        }
        POOL_COUNTS(OUTPUT)
    #undef OUTPUT
    printf("\n");
}

void resetObjectCount(ObjectCount* oc, int capacity) {
    oc->activeCount = 0;
    oc->capacity = capacity;
    oc->spike = 0;
}

void updateDebug(GameContext* ctx) {

    if (!ctx->debug.active) return;

    Debug* debug = &ctx->debug;

    bool hasChanged = false;

    #define OUTPUT(name)                                                                        \
        do {                                                                                    \
            if (debug->poolCount.name.showInDebug) {                                            \
                if (debug->poolCount.name.activeCount != ctx->objectPools.name.activeCount) {   \
                    hasChanged = true;                                                          \
                    debug->poolCount.name.activeCount = ctx->objectPools.name.activeCount;      \
                    if (debug->poolCount.name.activeCount > debug->poolCount.name.spike) {      \
                        debug->poolCount.name.spike = debug->poolCount.name.activeCount;        \
                    }                                                                           \
                }                                                                               \
            }                                                                                   \
        } while (0);

    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    if (debug->onlyOutputOnChange) {
        if (hasChanged) outputDebugToTerminal(debug);
    } else {
        debug->updateTimer += GetFrameTime();

        if (debug->updateTimer >= debug->outputFrequency) {
            debug->updateTimer = 0.0f;
            outputDebugToTerminal(debug);
        }  
    } 
}

bool updateObjectCountSection(ObjectCountSection* section) {
    
    bool isHovered = false;

    for (int i = 0; i < NUMBER_OF_POOL_COUNTS; i++) {
        bool checkboxHovered = updateCheckbox(&section->options[i].checkbox);

        if (checkboxHovered) isHovered = checkboxHovered;
    }

    return isHovered;
}

bool updateOutputOptionsSection(DebugOutputOptionsSection* section) {

    bool checkBoxHovered = updateCheckbox(&section->options.outputOnChangeCheckbox.checkbox);
    updateButton(&section->options.decreaseButton);
    updateButton(&section->options.increaseButton);

    if (
        checkBoxHovered ||
        section->options.decreaseButton.isHovered ||
        section->options.increaseButton.isHovered
    ) {
        return true;
    } else {
        return false;
    }
}
