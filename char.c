#include <stdio.h>
#include <stdint.h>
#include <ctype.h> // for isalpha and tolower

void charcounter(char *str, uint8_t *result);

void charcounter(char *str, uint8_t *result) {
    result[0] = 0; // vowel count
    result[1] = 0; // consonant count

    while (*str) {
        char ch = tolower(*str); // Convert character to lowercase for comparison
        if (isalpha(ch)) { // Check if the character is an alphabetic letter
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                result[0]++; // Increment vowel count
            } else {
                result[1]++; // Increment consonant count
            }
        }
        str++; // Move to the next character
    }
}

// Test function to demonstrate usage
int main() {
    char text[] = "Hello, World! This is a test sentence.";
    uint8_t counts[2] = {0}; // Array to hold the counts [vowels, consonants]

    charcounter(text, counts);

    printf("Vowels: %d, Consonants: %d\n", counts[0], counts[1]);
    return 0;
}
