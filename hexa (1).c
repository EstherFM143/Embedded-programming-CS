#include <stdint.h>
#include <math.h>
#include <stdio.h>

// Function prototype
uint8_t hexago(uint32_t luku);

uint8_t hexago(uint32_t luku) {
    // Calculate the discriminant
    double discriminant = 1 + 8 * luku;

    // Check if the discriminant is a perfect square
    double sqrtDisc = sqrt(discriminant);

    // If sqrtDisc is not an integer, return 0
    if (sqrtDisc != (int)sqrtDisc) {
        return 0;
    }

    // Calculate n
    double n = (1 + sqrtDisc) / 4;

    // Check if n is a positive integer
    if (n == (int)n && n > 0) {
        return 1; // It is a hexagonal number
    }

    return 0; // It is not a hexagonal number
}
