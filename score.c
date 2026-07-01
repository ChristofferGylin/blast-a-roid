#include "score.h"
#include "constants.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>
#include "gameContext.h"
#include "secrets.h"
#include <string.h>

static const int ASTEROID_POINTS_1 = 50;
static const int ASTEROID_POINTS_2 = 100;
static const int ASTEROID_POINTS_3 = 300;
static const int METAL_ASTEROID_POINTS = 1000;

void saveHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES]);

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

void initHighScores(Highscore* highscores[NUMBER_OF_HIGHSCORES]) {
    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        Highscore* entry = &highscores[i];

        entry->level = 0;
        entry->score = 0;
        strcpy("", entry->name);
        
    }
}

void resetTimeBonusMultiplier(GameContext* ctx) {
    
    Player* player = &ctx->player;
    
    if (player->timeBonusTimer == 0) return;

    if ((GetTime() * 1000.0) >= player->timeBonusTimer + TIME_BONUS_LIMIT + (ctx->pausTimer * 1000)) {
        player->timeBonusMultiplier = 1;
        player->timeBonusTimer = 0;
    }
}

void saveHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES]) {
    HighscoreSaveData saveFile;

    memcpy(saveFile.scores, highscores, NUMBER_OF_HIGHSCORES * sizeof(Highscore));
    saveFile.checksum = ComputeCRC32((unsigned char *)saveFile.scores, sizeof(saveFile.scores));
    saveFile.checksum ^= SECRET_NUMBER;

    SaveFileData("./data.dat", &saveFile, sizeof(saveFile));
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

