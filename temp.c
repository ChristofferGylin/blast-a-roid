#include <stdbool.h>
#include <stdio.h>

void compact_bools(bool *arr, int size) {
    int write = 0;

    // Move all true values forward
    for (int i = 0; i < size; i++) {
        if (arr[i]) {
            arr[write++] = true;
        }
    }

    // Fill the rest with false
    for (int i = write; i < size; i++) {
        arr[i] = false;
    }
}

int main() {

    bool arr[] = {true, false, true, false, false};
    int size = 5;

    compact_bools(arr, size);

    // Result: {true, true, false, false, false}

    for (int i = 0; i < 5; i++) {
        printf("\n");
        printf("%s ", arr[i] ? "true": "false");
        printf("\n");
    }

    return 0;
}