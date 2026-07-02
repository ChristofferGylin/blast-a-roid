#ifndef SCORE_H
#define SCORE_H

#include <inttypes.h>

#include "asteroid.h"
#include "player.h"

#define NUMBER_OF_HIGHSCORES 10

typedef struct Highscore {
    uint64_t score;
    char name[32];
    int level;
}Highscore;

typedef struct HighscoreSaveData {
    Highscore scores[NUMBER_OF_HIGHSCORES];
    uint32_t checksum;
}HighscoreSaveData;


void addScore(Player* player, Asteroid* ast);
bool checkHighscore(GameContext* ctx);
void initHighScores(Highscore highscores[NUMBER_OF_HIGHSCORES]);
void resetTimeBonusMultiplier(GameContext* ctx);
void updateLevelBonus(Player* player);
void updateTimeBonusMultiplier(Player* player);

#endif
