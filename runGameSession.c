#include "runGameSession.h"
#include "player.h"
#include "gameOver.h"
#include "gameloop.h"
#include "scoreScreen.h"

void runGameSession() {
    Player player = {0};
    initPlayer(&player);

    while (player.lives >= 0) {            
        gameLoop(&player);

        if (player.lives < 0) {
            gameOver(&player);
        } else {
            scoreScreen(&player);
        }
    }
}