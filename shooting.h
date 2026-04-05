#include "raylib.h"
#include <stdbool.h>

typedef struct {    
    Vector2 posistion;
    Vector2 direction;
    double created;
    bool destroyed;
}Shot;

