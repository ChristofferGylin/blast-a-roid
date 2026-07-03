#include "runGameSession.h"
#include "player.h"
#include "gameContext.h"
#include "gameOver.h"
#include "gameloop.h"
#include "scoreScreen.h"
#include "highscoreScreen.h"

bool runGameSession(GameContext* ctx) {
    initPlayer(&ctx->player);
    
    GameResult result = GAME_CONTINUE;

    while (ctx->player.lives >= 0) {            
        result = gameLoop(ctx);

        if (result == EXIT_TO_DESKTOP) {
            return false;
        } else if (result == EXIT_TO_MENU) {
            return true;
        }

        if (ctx->player.lives < 0) {
            gameOver(&ctx->player);
            result = highscoreScreen(ctx);

            if (result == EXIT_TO_DESKTOP) {
                return false;
            } else if (result == EXIT_TO_MENU) {
                return true;
            }

        } else {
            scoreScreen(ctx);
        }
    }

    return true;
}