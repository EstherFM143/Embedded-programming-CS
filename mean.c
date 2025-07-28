#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float mean(char *list);

float mean(char *list) {
    int sum = 0;
    int count = 0;

    // Uses strtok to split the string by commas
    char *token = strtok(list, ",");

    while (token != NULL) {
        // Converts each token to an integer and add it to the sum
        sum += atoi(token);
        count++;  // Keeps track of the number of integers

        // Moves to the next token
        token = strtok(NULL, ",");
    }

    // Returns the arithmetic mean
    if (count == 0) {
        return 0;  // Handles case where there are no numbers
    }

    return (float)sum / count;
}

// Test function
int main() {
    char data[] = "201,53,12,31,5";
    float result = mean(data);

    printf("The mean is: %.2f\n", result);
    return 0;
}
