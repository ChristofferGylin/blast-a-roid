#include "runGameSession.h"
#include "player.h"
#include "gameOver.h"
#include "gameloop.h"
#include "scoreScreen.h"

bool runGameSession() {
    Player player = {0};
    initPlayer(&player);
    GameResult gameLoopResult = GAME_CONTINUE;

    while (player.lives >= 0) {            
        gameLoopResult = gameLoop(&player);

        if (gameLoopResult == EXIT_TO_DESKTOP) {
            return false;
        } else if (gameLoopResult == EXIT_TO_MENU) {
            return true;
        }

        if (player.lives < 0) {
            gameOver(&player);
        } else {
            scoreScreen(&player);
        }
    }

    return true;
}