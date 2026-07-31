#ifndef DRAWING_H
#define DRAWING_H

#include "raylib.h"

typedef struct Drawing {
    Rectangle srcRect;
    Rectangle dstRect;
    RenderTexture2D renderTexture;
}Drawing;

void drawGame(Rectangle src, Rectangle dst, RenderTexture2D renderTexture);
void initDrawing(Drawing* drawing);

#endif
