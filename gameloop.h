#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "player.h"

typedef enum {
    GAME_CONTINUE,
    EXIT_TO_MENU,
    EXIT_TO_DESKTOP
}GameResult;

GameResult gameLoop(Player* player);

#endif