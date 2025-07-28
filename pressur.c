#include <inttypes.h>

uint32_t pressure(uint8_t xlsb, uint8_t lsb, uint8_t msb);

uint32_t pressure(uint8_t xlsb, uint8_t lsb, uint8_t msb) {
    // Combine the three 8-bit registers into a 20-bit value
    uint32_t press = ((uint32_t)msb << 12) | ((uint32_t)lsb << 4) | (xlsb >> 4);

    return press;
}
