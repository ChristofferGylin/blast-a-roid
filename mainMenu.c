#include "constants.h"
#include "colors.h"
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

void drawHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES], Rectangle container);

void drawHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES], Rectangle container) {
    int highscoreFontSize = 22;
    int highscoreFontSpacing = 6;
    int highscoreMarginY = 20;
    int highscoreMarginX = 10;
    int lineWidth = 3;

    int yPosition = container.y + highscoreMarginY;

    char number[3] = "99";
    char name[33] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char level[4] = "999";
    char score[21] = "99999999999999999999";

    Vector2 numberSize = MeasureTextEx(GetFontDefault(), number, highscoreFontSize, highscoreFontSpacing);
    Vector2 nameSize = MeasureTextEx(GetFontDefault(), name, highscoreFontSize, highscoreFontSpacing);
    Vector2 levelSize = MeasureTextEx(GetFontDefault(), level, highscoreFontSize, highscoreFontSpacing);
    Vector2 scoreSize = MeasureTextEx(GetFontDefault(), score, highscoreFontSize, highscoreFontSpacing);

    int availibleHeight = (container.height - numberSize.y - (highscoreMarginY * 2));
    int gapX = 20;
    int gapY = availibleHeight / (NUMBER_OF_HIGHSCORES -1);

    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        snprintf(number, sizeof(number), "%d", i + 1);

        if (highscores[i].score > 0) {
            snprintf(name, sizeof(name), "%s", highscores[i].name);
            snprintf(level, sizeof(level), "%d", highscores[i].level);
            snprintf(score, sizeof(score), "%" PRIu64, highscores[i].score);
        } else {
            strcpy(name, "");
            strcpy(level, "");
            strcpy(score, "");
        }

        int numberXPos = container.x +  highscoreMarginX + lineWidth;
        int nameXPos = numberXPos + numberSize.x + gapX;
        int scoreXPos = container.x + container.width - highscoreMarginY;
        int levelXPos = nameXPos + ((scoreXPos - nameXPos) / 2);

        Vector2 origin = {0,0};
        
        DrawTextPro(
            GetFontDefault(),
            number,
            (Vector2){numberXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
        );

        DrawTextPro(
            GetFontDefault(),
            name,
            (Vector2){nameXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
        );

        DrawTextPro(
            GetFontDefault(),
            level,
            (Vector2){levelXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
        );
        
        DrawTextPro(
            GetFontDefault(),
            score,
            (Vector2){scoreXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
        );

        yPosition += gapY;

    }
}

Rectangle drawLayoutContainers() {

    Rectangle background = {
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

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

    DrawRectangleGradientV(background.x, background.y, background.width, background.height, topColor, bottomColor);
    
    DrawTextPro(
        GetFontDefault(),
        "BLAST-A-ROID",
        logoPosition,
        (Vector2){0, 0},
        0,
        logoFontSize,
        logoFontSpacing,
        primaryColor
    );

    DrawTextPro(
        GetFontDefault(),
        "HIGHSCORES",
        highScoreHeadingPos,
        (Vector2){0, 0},
        0,
        logoFontSize,
        logoFontSpacing,
        primaryColor
    );

    Vector2 highscoresLinePos = {
        highScoreContainer.x + margin,
        highScoreHeadingPos.y + highScoreHeadingSize.y + 10
    };

    Vector2 highscoresLineSize = {
        highScoreContainer.width - (margin * 2),
        3
    };

    DrawRectangle(highscoresLinePos.x, highscoresLinePos.y, highscoresLineSize.x, highscoresLineSize.y, primaryColor);

    DrawRectangleRoundedLinesEx(highScoreContainer, getRoundness(highScoreContainer, roundnessRadius), 10, 3, primaryColor);
    DrawRectangleRoundedLinesEx(logoContainer, getRoundness(logoContainer, roundnessRadius), 10, 3, primaryColor);
    DrawRectangleRoundedLinesEx(menuContainer, getRoundness(menuContainer, roundnessRadius), 10, 3, primaryColor);

    Rectangle highscoreTextContainer;
    highscoreTextContainer.x = highScoreContainer.x;
    highscoreTextContainer.y = highscoresLinePos.y + highscoresLineSize.y;
    highscoreTextContainer.width = highScoreContainer.width;
    highscoreTextContainer.height = highScoreContainer.height - (highscoreTextContainer.y - (highScoreContainer.y + highscoresLineSize.y));

    return highscoreTextContainer;
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
            primaryColor
        );

        if (item->isHovered) {
            DrawRectangle(
                pos.x,
                pos.y + item->size.y + underLineOffset,
                item->size.x,
                underLineHeight,
                primaryColor
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
    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool isRunning = true;

    while (!WindowShouldClose() && isRunning) {
        updateMenu(&menu);

        if (isFadeOutComplete) {
            switch (menu.selected) {
                case -1: break;
                case 0: 
                    menu.selected = -1;
                    isRunning = runGameSession(ctx);
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
            Rectangle highscoreContainer = drawLayoutContainers();
            drawHighscores(ctx->highscores, highscoreContainer);
            drawMenu(&menu);
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (menu.selected != -1 && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }
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