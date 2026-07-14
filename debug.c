#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "debug.h"
#include "gameContext.h"
#include "raylib.h"
#include "ui.h"
#include "uiSizes.h"

static const int COUNT_OPTION_FONT_SIZE = 18;

void backButtonOnClick(void* userData);
void drawPoolCountOption(ObjectCount* option, char* title, Vector2 position);
void initObjectCountOption(ObjectCountOption* option, Vector2 position, char* title, bool* state);
void outputDebugToTerminal(Debug* debug);
void outputObjectCountToTerminal(const char* name, ObjectCount oc);
void resetObjectCount(ObjectCount* oc, int capacity);

static Rectangle drawLayoutContainers();

void backButtonOnClick(void* userData) {
    bool* exit = userData;

    *exit = true;
}

bool debugMenu(GameContext* ctx) {

    bool exit = false;
    bool applicationIsRunning = true;

    Button backButton;
    initButton(
        &backButton,
        (Rectangle){MENU_MARGIN * 2.0f, (MENU_MARGIN * 2.0f) + MENU_LINE_THICKNESS, 0, 0},
        BUTTON_FONT_SIZE, "BACK",
        backButtonOnClick,
        &exit
    );

    while (!WindowShouldClose()) {
        updateButton(&backButton);

        BeginDrawing();
            drawLayoutContainers();
            drawButton(&backButton);
        EndDrawing();

        if (exit) break;
    }

    if (WindowShouldClose()) applicationIsRunning = false;

    return applicationIsRunning;
}

void drawObjectCountOption(ObjectCountOption* option) {
    
    int yCenter = option->position.y + MENU_MARGIN + CHECKBOX_SIZE;
    const int COUNT_OPTION_FONT_SIZE = 18;
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), option->title, COUNT_OPTION_FONT_SIZE, MENU_FONT_SPACING);
    
    // Vector2 checkBoxPosition = {option->position.x + MENU_MARGIN, yCenter - CHECKBOX_SIZE / 2.0f};
    Vector2 textPosition = {
        option->checkbox.position.x + CHECKBOX_SIZE + (MENU_MARGIN / 2.0f),
        yCenter - (titleSize.y / 2.0f)
    };
    
    drawCheckbox(&option->checkbox);
}

Rectangle drawLayoutContainers() {

    Rectangle background = {
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    Rectangle mainContainer = {
        MENU_MARGIN,
        MENU_MARGIN,
        SCREEN_WIDTH - (MENU_MARGIN * 2),
        SCREEN_HEIGHT - (MENU_MARGIN * 2)
    };

    char headingText[] = "DEBUG OPTIONS";

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), headingText, MENU_HEADING_FONT_SIZE, MENU_HEADING_FONT_SPACING);

    Vector2 headingPos = {
        (SCREEN_WIDTH / 2) - (headingSize.x / 2),
        mainContainer.y + MENU_MARGIN + MENU_LINE_THICKNESS
    };

    DrawRectangleGradientV(background.x, background.y, background.width, background.height, topColor, bottomColor);

    DrawTextPro(
        GetFontDefault(),
        headingText,
        headingPos,
        (Vector2){0, 0},
        0,
        MENU_HEADING_FONT_SIZE,
        MENU_HEADING_FONT_SPACING,
        primaryColor
    );

    Vector2 linePos = {
        mainContainer.x,
        headingPos.y + headingSize.y + MENU_MARGIN
    };

    Vector2 lineSize = {
        mainContainer.width,
        MENU_LINE_THICKNESS,
    };

    DrawRectangle(linePos.x, linePos.y, lineSize.x, lineSize.y, primaryColor);

    DrawRectangleRoundedLinesEx(mainContainer, getRoundness(mainContainer, MENU_ROUNDNESS_RADIUS), MENU_ROUNDNESS_SEGMENTS, MENU_LINE_THICKNESS, primaryColor);

    Rectangle contentContainer;
    contentContainer.x = mainContainer.x;
    contentContainer.y = linePos.y + lineSize.y;
    contentContainer.width = mainContainer.width;
    contentContainer.height = mainContainer.height - (contentContainer.y - (mainContainer.y + lineSize.y));

    return contentContainer;
}

void initDebug(Debug* debug, bool active) {
    debug->active = active;
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
}

void initObjectCountOption(ObjectCountOption* option, Vector2 position, char* title, bool* state) {
    
    int yCenter = position.y + ((MENU_MARGIN + CHECKBOX_SIZE) / 2.0f);
    
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, COUNT_OPTION_FONT_SIZE, MENU_FONT_SPACING);
    
    Vector2 checkBoxPosition = {position.x + MENU_MARGIN, yCenter - CHECKBOX_SIZE / 2.0f};
    Vector2 titlePosition = {
        checkBoxPosition.x + CHECKBOX_SIZE + (MENU_MARGIN / 2.0f),
        yCenter - (titleSize.y / 2.0f)
    };
    
    initCheckbox(&option->checkbox, state, checkBoxPosition);
    strcpy(option->title, title);
    option->titlePosition;
}

void outputObjectCountToTerminal(const char* name, ObjectCount oc) {
    printf("%-21s Active count: %4d / %4d   Spike: %4d\n", name, oc.activeCount, oc.capacity, oc.spike);
}

void outputDebugToTerminal(Debug* debug) {

    #define OUTPUT(name) \
        outputObjectCountToTerminal(#name, debug->poolCount.name);

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

    Debug* debug = &ctx->debug;

    if (!debug->active) return;

    const float updateFrequency = 1.0f;

    debug->updateTimer += GetFrameTime();

    if (debug->updateTimer < updateFrequency) return;
    
    debug->updateTimer = 0.0f;

    #define OUTPUT(name)                                                     \
        do {                                                                 \
            debug->poolCount.name.activeCount = ctx->objectPools.name.activeCount; \
            if (debug->poolCount.name.activeCount > debug->poolCount.name.spike) { \
                debug->poolCount.name.spike = debug->poolCount.name.activeCount;    \
            }                                                                \
        } while (0);

    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    outputDebugToTerminal(debug);
}