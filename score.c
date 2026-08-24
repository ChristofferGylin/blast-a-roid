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

void loadHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES]);
void saveHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES]);

void addHighscore(Highscore highscores[NUMBER_OF_HIGHSCORES], Highscore newHighscore) {
    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        if (newHighscore.score > highscores[i].score) {
            for (int j = NUMBER_OF_HIGHSCORES - 1; j > i; j--) {
                highscores[j] = highscores[j - 1];
            }
            highscores[i] = newHighscore;
            break;
        }
    }

    saveHighscores(highscores);
}

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

NewHighscore checkHighscore(GameContext* ctx) {
    
    NewHighscore result = {0};

    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        if (ctx->player.score > ctx->highscores.scores[i].score) {
            result.hasNewHighscore = true;
            result.scoreIndex = i;
            break;
        }
    }

    return result;
}

void initHighScores(Highscores* highscores) {

    resetLastHighscore(highscores);
    
    if (FileExists("./data.dat")) {
        loadHighscores(highscores->scores);
        printf("Successfully loaded highscores from file\n");
    } else {

        printf("Error: Could not load highscores from file, recreating file...\n");

        resetHighscores(highscores);
    }
}

void loadHighscores(Highscore highscores[NUMBER_OF_HIGHSCORES]) {
    int size;
    HighscoreSaveData *scoresFromFile = (HighscoreSaveData *)LoadFileData("./data.dat", &size);

    if (scoresFromFile && size == sizeof(HighscoreSaveData)) {

        uint32_t hash = ComputeCRC32((unsigned char *)scoresFromFile->scores, sizeof(scoresFromFile->scores));
        hash ^= SECRET_NUMBER;

        if (hash == scoresFromFile->checksum) {
            memcpy(highscores, scoresFromFile->scores, sizeof(scoresFromFile->scores));
        } else {
            printf("Error: The data file have been modified or corrupted, could not load highscores in loadHighscores");
        }
        
    } else {
        printf("Error: Could not read highscores from file in loadHighscores");
    }

    UnloadFileData((unsigned char *)scoresFromFile);
}

void resetHighscores(Highscores* highscores) {
    for (int i = 0; i < NUMBER_OF_HIGHSCORES; i++) {
        Highscore* entry = &highscores->scores[i];

        entry->level = 0;
        entry->score = 0;
        entry->name[0] = '\0';
    }

    saveHighscores(highscores->scores);
}

void resetHighscoresCallback(void* userData) {
    Highscores* highscores = userData;
    resetHighscores(highscores);
}

void resetLastHighscore(Highscores* highscores) {
    highscores->hasNewHighscore = false;
    highscores->latestScoreIndex = 0;
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

    bool success = SaveFileData("./data.dat", &saveFile, sizeof(saveFile));

    if (!success) {
        printf("Error: Could not save highscores to file in saveHighscores");
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

