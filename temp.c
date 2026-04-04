#include <stdbool.h>
#include <stdlib.h>


typedef struct{
    int x, y, x;
} Thing;

typedef struct {
    bool allocated;
    Thing obj;
} PoolObject;

#define NUM_OBJECTS 10

PoolObject object_pool[NUM_OBJECTS] = {0};

Thing* BorrowThing(void) {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        if (object_pool[i].allocated == false) {
            object_pool[i].allocated = true;
            return &(object_pool[i].obj);
        }
    }
    return NULL;
}

void ReturnThing(Thing* t) {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        if (&(object_pool[i].obj) == t) {
            object_pool[i].allocated = false;
            return;
        } 
    }
}

int main() {
    Thing* thing = BorrowThing();
    // do stuff with thing
    ReturnThing(thing);

}