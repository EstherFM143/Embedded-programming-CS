#include <inttypes.h>

float temperature(uint16_t reg);

float temperature(uint16_t reg) {
    // Extract bits T0-T13 (14 bits) from the 16-bit register
    uint16_t temp_bits = (reg >> 2) & 0x3FFF;

    // Convert to temperature
    double temp = (double)temp_bits * 0.03125;

    return (float)temp;
}
