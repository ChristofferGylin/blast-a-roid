#ifndef SIDEBARS_H
#define SIDEBARS_H

#include <inttypes.h>

typedef struct Player Player;

void renderSidebars(Player* player);
int renderStats(uint64_t value, char title[], int startY);

#endif