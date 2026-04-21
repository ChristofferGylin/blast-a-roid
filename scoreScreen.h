#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <inttypes.h>
#include <stdbool.h>

typedef struct Player Player;

typedef struct BonusCountUpState {
    int bonusBuffer;
    int bonusBufferMultiplier;
    bool isZeroPadded;
}BonusCountUpState;

void bonusCountDown(uint64_t* src, uint64_t* target);
void bonusCountUp(uint64_t* src, uint64_t* target, BonusCountUpState* state);
int renderScoreLine(uint64_t value, char title[], int startY, bool hasUnderline, int bonusMultiplierLevel, bool isZeroPadded);
void scoreScreen(Player* player);

#endif