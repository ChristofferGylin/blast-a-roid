#include "constants.h"
#include "raylib.h"
#include "player.h"
#include "gameContext.h"
#include "gameloop.h"
#include "gameOver.h"
#include "scoreScreen.h"
#include <stdio.h>
#include "mainMenu.h"
#include <string.h>
#include "fader.h"
#include "runGameSession.h"
#include "utils.h"

int fontSize = 32;
int fontSpacing = 6;
int nextItemGap = 20;
int underLineHeight = 3;
int underLineOffset = 3;
int margin = 20;
int roundnessRadius = 12.0f;
int logoFontSize = 42;
int logoFontSpacing = 8;

void drawLayoutContainers() {

    Rectangle highScoreContainer = {
        margin,
        margin,
        ((SCREEN_WIDTH / 3) * 2) - (margin / 2) - margin,
        SCREEN_HEIGHT - (margin * 2)
    };

    Rectangle logoContainer = {
        SCREEN_WIDTH - (SCREEN_WIDTH / 3) + (margin / 2),
        margin,
        SCREEN_WIDTH / 3 - (margin / 2) - margin,
        (SCREEN_HEIGHT / 4) - (margin / 2) - margin
    };

    Rectangle menuContainer = {
        logoContainer.x,
        logoContainer.y + logoContainer.height + margin,
        logoContainer.width,
        ((SCREEN_HEIGHT / 4) * 3) - (margin / 2) - margin
    };
    
    Vector2 logoSize = MeasureTextEx(GetFontDefault(), "BLAST-A-ROID", logoFontSize, logoFontSpacing);

    Vector2 logoPosition = {
        logoContainer.x + (logoContainer.width / 2) - logoSize.x / 2,
        logoContainer.y + (logoContainer.height / 2) - logoSize.y / 2
    };

    Vector2 highScoreHeadingSize = MeasureTextEx(GetFontDefault(), "HIGHSCORES", logoFontSize, logoFontSpacing);

    Vector2 highScoreHeadingPos = {
        highScoreContainer.x + (highScoreContainer.width / 2) - highScoreHeadingSize.x / 2,
        highScoreContainer.y + margin
    };
    
    DrawTextPro(
        GetFontDefault(),
        "BLAST-A-ROID",
        logoPosition,
        (Vector2){0, 0},
        0,
        logoFontSize,
        logoFontSpacing,
        RAYWHITE
    );

    DrawTextPro(
        GetFontDefault(),
        "HIGHSCORES",
        highScoreHeadingPos,
        (Vector2){0, 0},
        0,
        logoFontSize,
        logoFontSpacing,
        RAYWHITE
    );

    Vector2 highscoresLinePos = {
        highScoreContainer.x + margin,
        highScoreHeadingPos.y + highScoreHeadingSize.y + 10
    };

    Vector2 highscoresLineSize = {
        highScoreContainer.width - (margin * 2),
        3
    };

    DrawRectangle(highscoresLinePos.x, highscoresLinePos.y, highscoresLineSize.x, highscoresLineSize.y, RAYWHITE);

    DrawRectangleRoundedLinesEx(highScoreContainer, getRoundness(highScoreContainer, roundnessRadius), 10, 3, RAYWHITE);
    DrawRectangleRoundedLinesEx(logoContainer, getRoundness(logoContainer, roundnessRadius), 10, 3, RAYWHITE);
    DrawRectangleRoundedLinesEx(menuContainer, getRoundness(menuContainer, roundnessRadius), 10, 3, RAYWHITE);
}

void drawMenu(Menu* menu) {
    for (int i = 0; i < menu->count; i++) {
        MenuItem* item = &menu->items[i];

        Vector2 pos = {
            item->basePosition.x,
            item->basePosition.y + menu->menuOffset
        };

        DrawTextPro(
            GetFontDefault(),
            item->text,
            pos,
            (Vector2){0, 0},
            0,
            fontSize,
            fontSpacing,
            RAYWHITE
        );

        if (item->isHovered) {
            DrawRectangle(
                pos.x,
                pos.y + item->size.y + underLineOffset,
                item->size.x,
                underLineHeight,
                RAYWHITE
            );
        }
    }
}

void initMenu(Menu* menu) {
    char titles[][32] = {
        "START GAME",
        "OPTIONS",
        "ABOUT",
        "EXIT"
    };

    int menuY = 0;

    for (int i = 0; i < 4; i++) {
        Vector2 size = MeasureTextEx(GetFontDefault(), titles[i], fontSize, fontSpacing);

        menu->items[i].basePosition = (Vector2){
            SCREEN_WIDTH - (SCREEN_WIDTH / 6) - size.x / 2,
            menuY
        };

        strcpy(menu->items[i].text, titles[i]);
        menu->items[i].size = size;
        menu->items[i].isHovered = false;

        menuY += size.y + nextItemGap;
    }

    menu->count = 4;

    int menuHeight = menuY - nextItemGap;
    menu->menuOffset = ((SCREEN_HEIGHT / 8) * 5) - (menuHeight / 2);

    menu->selected = -1;
}

void mainMenu(GameContext* ctx) {
    Menu menu;
    initMenu(&menu);
    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);
    bool isRunning = true;

    while (!WindowShouldClose() && isRunning) {
        updateMenu(&menu);

        if (menu.selected != -1 && faderArgs.fadeIn) {
            faderArgs.fadeIn = false;
            faderArgs.fadeComplete = false;
        }

        if (faderArgs.fadeComplete) {
            switch (menu.selected) {
                case -1: break;
                case 0: 
                    menu.selected = -1;
                    isRunning = runGameSession(ctx);
                    faderArgs.fadeIn = true;
                break;

                case 1:
                    // TODO: Options
                break;

                case 2: 
                    // TODO: About
                break;

                case 3:
                    isRunning = false;
                break;
        
                default: 
                    printf("Error: Invalid menu choice (%d) in main menu\n", menu.selected);
                break;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            drawLayoutContainers();
            drawMenu(&menu);
            fader(&faderArgs);
        EndDrawing();
    }
}

void updateMenu(Menu* menu) {
    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < menu->count; i++) {
        MenuItem* item = &menu->items[i];

        Rectangle rect = {
            item->basePosition.x,
            item->basePosition.y + menu->menuOffset,
            item->size.x,
            item->size.y
        };

        item->isHovered = CheckCollisionPointRec(mouse, rect);

        if (item->isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            menu->selected = i;
        }
    }
}