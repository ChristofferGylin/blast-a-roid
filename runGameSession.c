#include "runGameSession.h"
#include "player.h"
#include "gameContext.h"
#include "gameOver.h"
#include "gameloop.h"
#include "scoreScreen.h"

bool runGameSession(GameContext* ctx) {
    initPlayer(&ctx->player);
    GameResult gameLoopResult = GAME_CONTINUE;

    while (ctx->player.lives >= 0) {            
        gameLoopResult = gameLoop(ctx);

        if (gameLoopResult == EXIT_TO_DESKTOP) {
            return false;
        } else if (gameLoopResult == EXIT_TO_MENU) {
            return true;
        }

        if (ctx->player.lives < 0) {
            gameOver(&ctx->player);
        } else {
            scoreScreen(&ctx->player);
        }
    }

    return true;
}