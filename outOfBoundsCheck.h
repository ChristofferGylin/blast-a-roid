#ifndef OUTOFBOUNDSCHECK_H
#define OUTOFBOUNDSCHECK_H

#include "raylib.h"

bool checkOutOfBounds(Vector2 position, int size);
void handleOutOfBounds(Vector2* position, int size);

#endif