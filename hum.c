#include <stdint.h>
#include <inttypes.h>
#include <math.h>

// Function prototype
float humidity(uint16_t reg);

float humidity(uint16_t reg) {
    // Using double for higher precision to avoid rounding errors
    double humidityValue = (double)reg / pow(2, 16) * 100;
    return (float)humidityValue; // Cast to float for the return type
}
