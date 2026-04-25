#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "player.h"

typedef struct GameContext GameContext;

typedef enum {
    GAME_CONTINUE,
    EXIT_TO_MENU,
    EXIT_TO_DESKTOP
}GameResult;

GameResult gameLoop(GameContext* ctx);

#endif