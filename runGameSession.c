#include "runGameSession.h"
#include "player.h"
#include "gameOver.h"
#include "gameloop.h"
#include "scoreScreen.h"

bool runGameSession() {
    Player player = {0};
    initPlayer(&player);
    bool isRunning = true;

    while (player.lives >= 0 || !isRunning) {            
        isRunning = gameLoop(&player);

        if (!isRunning) return isRunning;

        if (player.lives < 0) {
            gameOver(&player);
        } else {
            scoreScreen(&player);
        }
    }

    return isRunning;
}