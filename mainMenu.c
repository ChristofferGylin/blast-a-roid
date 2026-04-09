#include "constants.h"
#include "raylib.h"
#include "player.h"
#include "gameloop.h"
#include "gameOver.h"
#include "scoreScreen.h"
#include <stdio.h>
#include "mainMenu.h"
#include <string.h>

int fontSize = 32;
int fontSpacing = 6;
int nextItemGap = 20;

void mainMenu() {
    Player player = {0};
    
    int menuChoice = -1;

    char titles[][11] = {
        "START GAME",
        "OPTIONS",
        "ABOUT",
        "EXIT"
    };

    MenuItem items[4] = {0};

    int menuY = 0;

    for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {
        items[i] = generateMenuItem(i, titles[i], &menuY);
    }

    int menuHeight = items[sizeof(items) / sizeof(MenuItem) - 1].position.y + items[sizeof(items) / sizeof(MenuItem) - 1].height.y;
    int menuYOffset = (SCREEN_HEIGHT / 2) - (menuHeight / 2);

    while (!WindowShouldClose()) {

        for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {

        }
        
        switch (menuChoice)
        {
        case -1: break;
        case 0: 
            menuChoice = -1;
            initPlayer(&player);
            while (player.lives >= 0) {
                
                gameLoop(&player);

                if (player.lives < 0) {
                    gameOver(&player);
                } else {
                    scoreScreen(&player);
                }
            }
            break;

        case 1:
            // TODO: Options
            break;

        case 2: 
            // TODO: About
            break;

        case 3:
            CloseWindow();
            break;
        
        default: 
            printf("Error: Invalid menu choice (%d) in main menu\n", menuChoice);
            break;
        }
        
        BeginDrawing();

            for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {
                MenuItem* item = &items[i];
                DrawTextPro(GetFontDefault(), item->text, (Vector2){item->position.x, item->position.y + menuYOffset}, (Vector2){0, 0}, 0, fontSize, fontSpacing, RAYWHITE);
            }
        EndDrawing();
    }
}

MenuItem generateMenuItem(int number, char title[], int* startY) {

    MenuItem newMenuItem = {0};
    strcpy(newMenuItem.text, title);
    
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, fontSize, fontSpacing);

    newMenuItem.position = (Vector2){
        SCREEN_WIDTH - (SCREEN_WIDTH / 4) - (titleSize.x / 2),
        *startY
    };

    newMenuItem.height = (Vector2) {
        titleSize.x,
        titleSize.y
    };

    *startY = newMenuItem.position.y + titleSize.y + nextItemGap;

    return newMenuItem;
}