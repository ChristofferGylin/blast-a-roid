#include "asteroid.h"
#include <stdlib.h>

void initAsteroidArray(AsteroidArray* arr, int capacity) {
    arr->data = malloc(sizeof(Asteroid) * capacity);
    arr->size = 0;
    arr->capacity = capacity;
}

void addAsteroid(AsteroidArray* arr, Asteroid ast) {
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

void removeAsteroid(AsteroidArray* arr, int index) {
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