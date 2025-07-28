#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void shuffle(uint8_t *list, uint16_t list_size);

void shuffle(uint8_t *list, uint16_t list_size) {
    uint8_t shuffled[list_size]; // Creates a temporary array
    uint16_t remaining = list_size;

    // Repeats till all elements are moved to the shuffled list
    for (uint16_t i = 0; i < list_size; i++) {
        // Picks a random index between 0 and remaining - 1
        uint16_t random_index = rand() % remaining;

        // Places the element from the original list at the chosen index into the shuffled list
        shuffled[i] = list[random_index];

        // Removes the selected element from the original list by shifting remaining elements left
        for (uint16_t j = random_index; j < remaining - 1; j++) {
            list[j] = list[j + 1];
        }

        // Reduces the count of remaining items
        remaining--;
    }

    // the shuffled result back into the original list
    for (uint16_t i = 0; i < list_size; i++) {
        list[i] = shuffled[i];
    }
}

int main() {
    // Initializes the random number generator
    srand(time(NULL));

    // Creates a sample array
    uint8_t list[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint16_t list_size = sizeof(list) / sizeof(list[0]);

    // Prints the original array
    printf("Original array: ");
    for (int i = 0; i < list_size; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");

    // Shuffles the array
    shuffle(list, list_size);

    // Prints the shuffled array
    printf("Shuffled array: ");
    for (int i = 0; i < list_size; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");

    return 0;
}
