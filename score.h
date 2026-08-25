#ifndef SCORE_H
#define SCORE_H

#include <inttypes.h>
#include <stdbool.h>

#include "asteroid.h"
#include "player.h"

typedef struct GameContext GameContext;

#define NUMBER_OF_HIGHSCORES 10
#define MAX_NAME_LENGTH 16

typedef struct Highscore {
    uint64_t score;
    char name[MAX_NAME_LENGTH + 1];
    int level;
}Highscore;

typedef struct Highscores {
    Highscore scores[NUMBER_OF_HIGHSCORES];
    bool hasNewHighscore;
    int latestScoreIndex;
}Highscores;

typedef struct HighscoreSaveData {
    Highscore scores[NUMBER_OF_HIGHSCORES];
    uint32_t checksum;
}HighscoreSaveData;

typedef struct NewHighscore {
    bool hasNewHighscore;
    int scoreIndex;
}NewHighscore;

void addHighscore(Highscore highscores[NUMBER_OF_HIGHSCORES], Highscore newHighscore);
void addScore(Player* player, Asteroid* ast);
NewHighscore checkHighscore(GameContext* ctx);
void initHighScores(Highscores* highscores);
void resetHighscores(Highscores* highscores);
void resetLastHighscore(Highscores* highscores);
void resetTimeBonusMultiplier(GameContext* ctx);
void updateLevelBonus(Player* player);
void updateTimeBonusMultiplier(Player* player);

#endif
