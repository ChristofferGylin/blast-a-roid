#ifndef DRAWING_H
#define DRAWING_H

#include "raylib.h"

typedef struct Rendering {
    Rectangle srcRect;
    Rectangle dstRect;
    RenderTexture2D renderTexture;
}Rendering;

Rectangle getRenderRect(float aspectWidth, float aspectHeight);
void initRendering(Rendering* rendering);
void renderToScreen(Rendering* rendering);

#endif
