#include <stdio.h>
#include <stdint.h>

void movavg(float *array, uint8_t array_size, uint8_t window_size);

void movavg(float *array, uint8_t array_size, uint8_t window_size) {
    if (array == NULL || array_size == 0 || window_size == 0 || window_size > array_size) {
        return;  // Invalid input parameters
    }

    for (uint8_t i = 0; i <= array_size - window_size; i++) {
        float sum = 0.0;
        for (uint8_t j = 0; j < window_size; j++) {
            sum += array[i + j];
        }
        float average = sum / window_size;

        // Prints the average with two decimal places
        printf("%.2f", average);

        // Prints comma if it's not the last average
        if (i < array_size - window_size) {
            printf(",");
        }
    }
    printf("\n");
}

int main() {
    float data[5] = {1.0, 2.0, 4.0, 6.0, 9.0};
    uint8_t array_size = 5;
    uint8_t window_size = 3;

    printf("Original array: ");
    for (int i = 0; i < array_size; i++) {
        printf("%.2f ", data[i]);
    }
    printf("\n");

    printf("Moving average: ");
    movavg(data, array_size, window_size);

    return 0;
}
