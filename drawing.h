#ifndef DRAWING_H
#define DRAWING_H

#include "raylib.h"

Vector2 getRenderSize(float aspectWidth, float aspectHeight);
void renderToScreen(RenderTexture2D renderTexture);

#endif
