#include "raylib.h"
#include "asteroid.h"
#include <stdlib.h>

float spriteWidth = 32.0f;
float spriteHeight = 32.0f;

void initAsteroids(AsteroidArray* arr, int number) {
    for (int i = 0; i < number; i++) {
        Vector2 position;

        if (i % 2 == 0) {
            position.x = -32.0f;
            position.y = GetRandomValue(0, GetScreenHeight());
        } else {
            position.x = GetRandomValue(0, GetScreenWidth());
            position.y = -32.0f;
        }

        Asteroid ast = {
            0,
            GetRandomValue(50, 600),
            {GetRandomValue(50, 400), GetRandomValue(50, 400)},
            position
        };

        addAsteroidToArray(arr, ast);
    }
}

void initAsteroidArray(AsteroidArray* arr, int capacity) {
    arr->data = malloc(sizeof(Asteroid) * capacity);
    arr->size = 0;
    arr->capacity = capacity;
}

void addAsteroidToArray(AsteroidArray* arr, Asteroid ast) {
    if (arr->size >= arr->capacity)
    {
        arr->capacity *= 2;
        Asteroid* newData = realloc(arr->data, sizeof(Asteroid) * arr->capacity);
        if (newData != NULL)
        {
            arr->data = newData;
        }
    }
    arr->data[arr->size++] = ast;
}

void removeAsteroidFromArray(AsteroidArray* arr, int index) {
    if (index < 0 || index >= arr->size) return;

    arr->data[index] = arr->data[arr->size - 1];
    arr->size--;
}

void freeAsteroidArray(AsteroidArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}