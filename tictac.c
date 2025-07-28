#include <stdio.h>
#include <stdint.h>

int8_t tictactoe_check(int8_t * gameboard, int win_len);

int8_t tictactoe_check(int8_t* gameboard, int win_len) {
    const int board_size = 10;
    int8_t winner = 0;

    // Helper function to check a line for a win
    int8_t check_line(int start, int step, int length) {
        int8_t first = gameboard[start];
        if (first == 0) return 0;
        for (int i = 1; i < length; i++) {
            if (gameboard[start + i * step] != first) return 0;
        }
        return first;
    }

    // Check rows and columns
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j <= board_size - win_len; j++) {
            int8_t row_result = check_line(i * board_size + j, 1, win_len);
            int8_t col_result = check_line(j * board_size + i, board_size, win_len);
            if (row_result) {
                if (winner && winner != row_result) return 0; // Two winners
                winner = row_result;
            }
            if (col_result) {
                if (winner && winner != col_result) return 0; // Two winners
                winner = col_result;
            }
        }
    }

    // Check diagonals
    for (int i = 0; i <= board_size - win_len; i++) {
        for (int j = 0; j <= board_size - win_len; j++) {
            int8_t diag1_result = check_line(i * board_size + j, board_size + 1, win_len);
            int8_t diag2_result = check_line(i * board_size + (j + win_len - 1), board_size - 1, win_len);
            if (diag1_result) {
                if (winner && winner != diag1_result) return 0; // Two winners
                winner = diag1_result;
            }
            if (diag2_result) {
                if (winner && winner != diag2_result) return 0; // Two winners
                winner = diag2_result;
            }
        }
    }

    return winner;
}

int main() {
    int8_t gameboard[100] = {0}; // Initialize empty board
    int win_len = 5; // Set win length

    // Set up a test board (X wins diagonally)
    gameboard[0] = 1;  // X
    gameboard[11] = 1; // X
    gameboard[22] = 1; // X
    gameboard[33] = 1; // X
    gameboard[44] = 1; // X

    gameboard[1] = 2;  // O
    gameboard[2] = 2;  // O
    gameboard[3] = 2;  // O
    gameboard[4] = 2;  // O

    int8_t result = tictactoe_check(gameboard, win_len);

    printf("Game result: ");
    switch(result) {
        case 0:
            printf("No winner or invalid board\n");
            break;
        case 1:
            printf("X wins\n");
            break;
        case 2:
            printf("O wins\n");
            break;
    }

    return 0;
}
