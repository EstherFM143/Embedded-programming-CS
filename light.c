#include <inttypes.h>
#include <math.h>

float light(uint16_t reg);

float light(uint16_t reg) {
    // Extract bits E0-E3 (4 bits)
    uint16_t E = (reg >> 12) & 0x000F;

    // Extract bits R0-R11 (12 bits)
    uint16_t R = reg & 0x0FFF;

    // Calculate illuminance
    double lux = 0.01 * pow(2, E) * R;

    return (float)lux;
}
