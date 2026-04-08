#ifndef SCORESCREEN_H
#define SCORESCREEN_H

#include <inttypes.h>

typedef struct Player Player;

int renderScoreLine(uint64_t value, char title[], int startY, bool hasUnderline);
void scoreScreen(Player* player);

#endif