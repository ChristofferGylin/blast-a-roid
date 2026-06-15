#include "score.h"
#include "constants.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>
#include "gameContext.h"

static const int ASTEROID_POINTS_1 = 50;
static const int ASTEROID_POINTS_2 = 100;
static const int ASTEROID_POINTS_3 = 300;
static const int METAL_ASTEROID_POINTS = 1000;

void addScore(Player* player, Asteroid* ast) {
    int score = 0;
    
    switch (ast->type) {
        case ASTEROID_LEVEL_1:
            score = ASTEROID_POINTS_1;
            break;
        
        case ASTEROID_LEVEL_2:
            score = ASTEROID_POINTS_2;
            break;
                    
        case ASTEROID_LEVEL_3:
            score = ASTEROID_POINTS_3;
            break;
                    
        case METAL_ASTEROID:
            score = METAL_ASTEROID_POINTS;
            break;
                        
        default:
            break;
    }
    
    player->score += score;
    player->levelBonus += score * player->timeBonusMultiplier;

    updateTimeBonusMultiplier(player);
}

void resetTimeBonusMultiplier(GameContext* ctx) {
    
    Player* player = &ctx->player;
    
    if (player->timeBonusTimer == 0) return;

    if (((GetTime() - ctx->pausTimer) * 1000.0) >= player->timeBonusTimer + TIME_BONUS_LIMIT) {
        player->timeBonusMultiplier = 1;
        player->timeBonusTimer = 0;
    }
}

void updateLevelBonus(Player* player) {

    if (player->levelBonus == 0) return;

    if (GetTime() >= player->levelBonusTimer + 1) {
        uint64_t newBonus = player->levelBonus - 10;

        if (newBonus < 0) {
            player->levelBonus = 0;
        } else {
            player->levelBonus = newBonus;
        }

        player->levelBonusTimer = GetTime();
    }
}

void updateTimeBonusMultiplier(Player* player) {
    player->timeBonusTimer = GetTime() * 1000.0;

    if (player->timeBonusMultiplier < 10) {
        player->timeBonusMultiplier++;
    }
}

