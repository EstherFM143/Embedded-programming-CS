#include <stdint.h>
#include <stdio.h>

// Function prototype
int64_t factorial(int8_t n);

int64_t factorial(int8_t n) {
    if (n < 0 || n > 20) {
        return -1;
    }

    int64_t result = 1;
    for (int8_t i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

int main() {
    for (int8_t i = 0; i <= 21; i++) {
        int64_t result = factorial(i);
        if (result == -1) {
            printf("factorial(%d) = Error: Input out of range\n", i);
        } else {
            printf("factorial(%d) = %ld\n", i, result);
        }
    }
    return 0;
}
