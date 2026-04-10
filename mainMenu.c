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
int underLineHeight = 3;
int underLineOffset = 3;

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
            SCREEN_WIDTH / 2 - size.x / 2,
            menuY
        };

        strcpy(menu->items[i].text, titles[i]);
        menu->items[i].size = size;
        menu->items[i].isHovered = false;

        menuY += size.y + nextItemGap;
    }

    menu->count = 4;

    int menuHeight = menuY - nextItemGap;
    menu->menuOffset = (SCREEN_HEIGHT / 2) - (menuHeight / 2);

    menu->selected = -1;
}

void mainMenu() {
    Menu menu;
    initMenu(&menu);

    Player player = {0};

    while (!WindowShouldClose()) {
        updateMenu(&menu);

        switch (menu.selected) {
            case -1: break;
            case 0: 
                menu.selected = -1;
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
                printf("Error: Invalid menu choice (%d) in main menu\n", menu.selected);
            break;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            drawMenu(&menu);
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

// void mainMenu() {
//     Player player = {0};
    
//     int menuChoice = -1;

//     char titles[][11] = {
//         "START GAME",
//         "OPTIONS",
//         "ABOUT",
//         "EXIT"
//     };

//     MenuItem items[sizeof(titles) / sizeof(titles[0])] = {0};

//     int menuY = 0;

//     for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {
//         items[i] = generateMenuItem(i, titles[i], &menuY);
//     }

//     int menuHeight = items[sizeof(items) / sizeof(MenuItem) - 1].position.y + items[sizeof(items) / sizeof(MenuItem) - 1].size.y;
//     int menuYOffset = (SCREEN_HEIGHT / 2) - (menuHeight / 2);

//     Rectangle collisionRects[sizeof(titles) / sizeof(titles[0])];

//     for (int i = 0; i < sizeof(collisionRects) / sizeof(Rectangle); i++) {
//         MenuItem* item = &items[i];
//         item->position.y += menuYOffset;
//         collisionRects[i] = (Rectangle) {
//             item->position.x,
//             item->position.y,
//             item->size.x,
//             item->size.y
//         };
//     }

//     while (!WindowShouldClose()) {

//         for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {
//             if (CheckCollisionPointRec(GetMousePosition(), collisionRects[i])) {
//                 items[i].isUnderlined = true;

//                 if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//                     menuChoice = i;
//                 }
//             } else {
//                 items[i].isUnderlined = false;
//             }
//         }
        
//         switch (menuChoice)
//         {
//         case -1: break;
//         case 0: 
//             menuChoice = -1;
//             initPlayer(&player);
//             while (player.lives >= 0) {
                
//                 gameLoop(&player);

//                 if (player.lives < 0) {
//                     gameOver(&player);
//                 } else {
//                     scoreScreen(&player);
//                 }
//             }
//             break;

//         case 1:
//             // TODO: Options
//             break;

//         case 2: 
//             // TODO: About
//             break;

//         case 3:
//             CloseWindow();
//             break;
        
//         default: 
//             printf("Error: Invalid menu choice (%d) in main menu\n", menuChoice);
//             break;
//         }
        
//         BeginDrawing();

//             for (int i = 0; i < sizeof(items) / sizeof(MenuItem); i++) {
//                 MenuItem* item = &items[i];
//                 DrawTextPro(GetFontDefault(), item->text, (Vector2){item->position.x, item->position.y}, (Vector2){0, 0}, 0, fontSize, fontSpacing, RAYWHITE);
//                 if (item->isUnderlined) {
//                     DrawRectangle(item->position.x, item->position.y + item->size.y + underLineHeight, item->size.x, underLineHeight, RAYWHITE);
//                 }
//             }
//         EndDrawing();
//     }
// }

// MenuItem generateMenuItem(int number, char title[], int* startY) {

//     MenuItem newMenuItem = {0};
//     strcpy(newMenuItem.text, title);
    
//     Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, fontSize, fontSpacing);

//     newMenuItem.position = (Vector2){
//         SCREEN_WIDTH - (SCREEN_WIDTH / 4) - (titleSize.x / 2),
//         *startY
//     };

//     newMenuItem.size = (Vector2) {
//         titleSize.x,
//         titleSize.y
//     };

//     *startY = newMenuItem.position.y + titleSize.y + nextItemGap;

//     return newMenuItem;
// }