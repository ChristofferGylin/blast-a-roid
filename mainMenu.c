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

void drawHighscores(Highscores* highscores, Rectangle container, HighlightTimers* highlightTimers);
void drawMenu(Menu* menu);
static Rectangle drawLayoutContainers();
void initHighlightTimers(HighlightTimers* timers);
void initMenu(GameContext* ctx, Menu* menu);
void updateHighlightTimers(GameContext* ctx, HighlightTimers* timers);
void updateMenu(Menu* menu);


void drawHighscores(Highscores* highscores, Rectangle container, HighlightTimers* highlightTimers) {

    int highscoreFontSize = 22;
    int highscoreFontSpacing = 6;
    int highscoreMarginY = 20;
    int highscoreMarginX = 35;
    int lineWidth = 3;

    char number[4] = "10.";
    char name[MAX_NAME_LENGTH + 1] = "AAAAAAAAAAAAAAAA";
    char level[4] = "999";
    char score[21] = "99999999999999999999";

    char numberTitle[] = "#";
    char nameTitle[] = "NAME";
    char levelTitle[] = "LVL";
    char scoreTitle[] = "SCORE";

    Vector2 numberSizeMax = MeasureTextEx(GetFontDefault(), number, highscoreFontSize, highscoreFontSpacing);
    Vector2 nameSizeMax = MeasureTextEx(GetFontDefault(), name, highscoreFontSize, highscoreFontSpacing);
    Vector2 scoreSizeMax = MeasureTextEx(GetFontDefault(), score, highscoreFontSize, highscoreFontSpacing);
    Vector2 levelSizeMax = MeasureTextEx(GetFontDefault(), level, highscoreFontSize, highscoreFontSpacing);

    Vector2 numberTitleSize = MeasureTextEx(GetFontDefault(), numberTitle, highscoreFontSize, highscoreFontSpacing);
    Vector2 nameTitleSize = MeasureTextEx(GetFontDefault(), nameTitle, highscoreFontSize, highscoreFontSpacing);
    Vector2 scoreTitleSize = MeasureTextEx(GetFontDefault(), scoreTitle, highscoreFontSize, highscoreFontSpacing);
    Vector2 levelTitleSize = MeasureTextEx(GetFontDefault(), levelTitle, highscoreFontSize, highscoreFontSpacing);

    int availibleHeight = (container.height - (levelSizeMax.y * 2) - (highscoreMarginY * 3));
    int gapX = 20;
    int gapY = availibleHeight / (NUMBER_OF_HIGHSCORES -1);

    int numberXPosBase = container.x +  highscoreMarginX + lineWidth;
    int nameXPosBase = container.x +  highscoreMarginX + lineWidth + gapX;
    int levelXPosBase = container.x + container.width - highscoreMarginX - lineWidth;
    int scoreXPosBase = container.x + container.width - highscoreMarginX - lineWidth - levelSizeMax.x - gapX;

    int numberTitleXPos = numberXPosBase - (numberSizeMax.x / 2.0f); 
    int nameTitleXPos = nameXPosBase;
    int levelTitleXPos = levelXPosBase - (levelTitleSize.x / 2.0f);
    int scoreTitleXPos = scoreXPosBase - scoreTitleSize.x;

    int yPosition = container.y;

    Vector2 origin = {0,0};

    Rectangle titlesBackground = {container.x, yPosition, container.width, nameTitleSize.y + highscoreMarginY};

    DrawRectanglePro(titlesBackground, origin, 0, primaryColorDimmed30);

    yPosition += + (highscoreMarginY / 2);

    DrawTextPro(
            GetFontDefault(),
            numberTitle,
            (Vector2){numberTitleXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
    );

    DrawTextPro(
            GetFontDefault(),
            nameTitle,
            (Vector2){nameTitleXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
    );

    DrawTextPro(
            GetFontDefault(),
            scoreTitle,
            (Vector2){scoreTitleXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
    );

    DrawTextPro(
            GetFontDefault(),
            levelTitle,
            (Vector2){levelTitleXPos, yPosition},
            origin,
            0,
            highscoreFontSize,
            highscoreFontSpacing,
            primaryColor
    );

    yPosition += nameTitleSize.y + (highscoreMarginY / 2) ;

    Vector2 dividerLineStart = {container.x, yPosition};
    Vector2 dividerLineEnd = {container.x + container.width, yPosition};

    DrawLineEx(dividerLineStart, dividerLineEnd, 1, primaryColor);

    yPosition += highscoreMarginY;

    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        snprintf(number, sizeof(number), "%d.", i + 1);

        if (highscores->scores[i].score > 0) {
            snprintf(name, sizeof(name), "%s", highscores->scores[i].name);
            snprintf(level, sizeof(level), "%d", highscores->scores[i].level);
            snprintf(score, sizeof(score), "%" PRIu64, highscores->scores[i].score);
        } else {
            strcpy(name, "");
            strcpy(level, "");
            strcpy(score, "");
        }

        Vector2 levelSize = MeasureTextEx(GetFontDefault(), level, highscoreFontSize, highscoreFontSpacing);
        Vector2 scoreSize = MeasureTextEx(GetFontDefault(), score, highscoreFontSize, highscoreFontSpacing);
        Vector2 numberSize = MeasureTextEx(GetFontDefault(), number, highscoreFontSize, highscoreFontSpacing);

        int numberXPos = numberXPosBase - numberSize.x;
        int levelXPos = levelXPosBase - levelSize.x;
        int scoreXPos = scoreXPosBase - scoreSize.x;

        if (highscores->hasNewHighscore && highscores->latestScoreIndex == i && highlightTimers->active && highlightTimers->blinkVisible) {

            Rectangle highlight = {container.x, yPosition - (highscoreMarginY / 2.0f), container.width, levelSize.y + highscoreMarginY};

            DrawRectanglePro(highlight, origin, 0, primaryColorDimmed30);   
        }
        
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
            (Vector2){nameXPosBase, yPosition},
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
        highScoreContainer.x,
        highScoreHeadingPos.y + highScoreHeadingSize.y + 10
    };

    Vector2 highscoresLineSize = {
        highScoreContainer.width,
        1
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

void initHighlightTimers(HighlightTimers* timers) {
    timers->activeTimer = 0.0f;
    timers->blinkTimer = 0.0f;
    timers->active = true;
    timers->blinkVisible = true;
}

void initMenu(GameContext* ctx, Menu* menu) {

    char titles[][32] = {
        "START GAME",
        "OPTIONS",
        "ABOUT",
        "EXIT",
        "DEBUG"
    };

    int menuY = 0;

    int numberOfItems = ctx->debug.active ? MAX_NUMBER_OF_MAIN_MENU_ITEMS : MAX_NUMBER_OF_MAIN_MENU_ITEMS - 1;

    for (int i = 0; i < numberOfItems; i++) {
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

    menu->count = numberOfItems;

    int menuHeight = menuY - nextItemGap;
    menu->menuOffset = ((SCREEN_HEIGHT / 8) * 5) - (menuHeight / 2);

    menu->selected = -1;
}

void mainMenu(GameContext* ctx) {
    Menu menu;
    HighlightTimers highlightTimers;
    initMenu(ctx, &menu);
    initHighlightTimers(&highlightTimers);

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool isRunning = true;

    while (!WindowShouldClose() && isRunning) {
        updateMenu(&menu);
        updateHighlightTimers(ctx, &highlightTimers);

        if (isFadeOutComplete) {
            switch (menu.selected) {
                case -1: break;
                case 0: 
                    menu.selected = -1;
                    initHighlightTimers(&highlightTimers);
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

                case 4:
                    if (ctx->debug.active) {
                        // Open debug menu
                        menu.selected = -1;
                        break;
                    }
                default: 
                    printf("Error: Invalid menu choice (%d) in main menu\n", menu.selected);
                break;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            Rectangle highscoreContainer = drawLayoutContainers();
            drawHighscores(&ctx->highscores, highscoreContainer, &highlightTimers);
            drawMenu(&menu);
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (menu.selected != -1 && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }
        EndDrawing();
    }
}

void updateHighlightTimers(GameContext* ctx, HighlightTimers* timers) {
    
    const float HIGHLIGHT_BLINK_TIME = 0.5f;
    const float HIGHLIGHT_ACTIVE_TIME = 10.0f;
    
    if (!ctx->highscores.hasNewHighscore || !timers->active) return;

    timers->activeTimer += GetFrameTime();
    timers->blinkTimer += GetFrameTime();

    if (timers->activeTimer >= HIGHLIGHT_ACTIVE_TIME) {
        timers->active = false;
        timers->activeTimer = 0.0f;
        return;
    }

    if (timers->blinkTimer >= HIGHLIGHT_BLINK_TIME) {
        timers->blinkVisible = !timers->blinkVisible;
        timers->blinkTimer = 0.0f;
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