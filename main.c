#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"
#include "engine.h"


#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

/* Program Functions' Declaration */

void showMenu();
int getSafeCellsFromConfigFile(char *fileName, int *safeCells);


int main(int argc, char const *argv[])
{
    /* Program variables */
    // Default values for 'Board Presentation Mode', 'Number of Lines' and 'Number of Columns'
    unsigned int boardPresentationMode = 0, linesNum = 3, columnsNum = 7;
    int safeCells[MAX_CELLS] = {0};  // Stores the safe cells read from the config file
    unsigned int totalCells;  // Number of total cells
    list boardCells;  // List struct to store all board cells data
    char inputOption;  // Stores user input option
    bool player1;  // Holds the player for the current play
    unsigned int dicesValue;  // Holds dices value for each move
    int gameOver;  // Holds the return of the 'checkGameWin' function

    // Initializes random seed
    srand(1);

    // Gets program args and checks if they're valid
    for (int i = 1; i < argc; i++) {
        // Checks if 'Board Presentation Mode' argument is set and valid
        if (i == 1 && (atoi(argv[i]) == 0 || atoi(argv[i]) == 1)) {
            boardPresentationMode = atoi(argv[i]);
        }

        // Checks if the 'Number of lines' is set and valid
        if (i == 2 && atoi(argv[i]) >= MIN_ROWS && atoi(argv[i]) % 2 != 0) {
            linesNum = atoi(argv[i]);
        }

        // Checks if the 'Number of Columns' is set and valid
        if (i == 3 && atoi(argv[i]) > MIN_COLS) {
            columnsNum = atoi(argv[i]);
        }

        // Checks if the 'Configuration File' is present and reads its content
        if (i == 4) {
            // Reads safe cells from config file and stores them in an array
            int getSafeCells = getSafeCellsFromConfigFile((char*)argv[i], safeCells);

            // Exits program if it fails to get safe cells from the config file
            if (getSafeCells == 1) {
                return 0;
            }
        }
    }

    // Updates the number of total cells based on given arguments
    totalCells = linesNum * 2 + (columnsNum - 2) * 2;

    // Initializes the board cells list
    initializeCellsList(&boardCells);

    // Adds cells to board (Board Setup)
    boardSetup(&boardCells, safeCells, totalCells);
    
    // Prints game info for the first time
    boardPrint(linesNum, columnsNum, boardCells, boardPresentationMode);  // Prints board
    showMenu();  // Prints menu
    player1 = true;  // Sets player 1 as the starting player

    // Game Loop
    do {
        /* 
            Checks if game is over (i.e. if a player won)
            It it is, prints the winner and exits program
        */
        gameOver = checkGameWin(&boardCells, totalCells);

        // Checks if P1 won
        if (gameOver == 1) {
            puts(PL1_WINS);
            return 0;
        }

        // Checks if P2 won
        if (gameOver == 1) {
            puts(PL2_WINS);
            return 0;
        }

        // Prints current player move
        if (player1) {
            puts(PL1_MOVE);
        } else {
            puts(PL2_MOVE);
        }

        // Rolls dices for current player move and prints the value
        dicesValue = rolldice(2);
        printf("%s%d\n", PL_DICE, dicesValue);
        
        printf(">");  // Input cursor
        scanf(" %c", &inputOption);  // Reads user input

        switch (inputOption) {
            case 'h':
                showMenu();
                break;

            case 's':
                // Prints end game message and exits
                puts("Fim do jogo");
                // Skips to the end
                break;
            
            default:
                // Checks if the inserted pawn is valid
                if (validPawn(inputOption, player1) && isPawnMovable(inputOption, &boardCells, player1)) {
                    makePlay(&boardCells, inputOption, dicesValue);

                    // Changes player move after previous play is finished
                    player1 = !player1;
                } else {
                    // Invalid option ERROR message
                    puts(INVAL_MOVE);
                }
            
                break;
        }

        // Prints the board again after the play
        if (inputOption != 's') {
            boardPrint(linesNum, columnsNum, boardCells, boardPresentationMode);  // Prints board
        }

    } while (inputOption != 's');
    
    return 0;
}


/* Program Functions */

/**
 * @brief Prints the game menu.
 *  
 */
void showMenu() {
    puts("+------------------------------------+");
    puts("|         Nao Te Constipes           |");
    puts("+------------------------------------+");
    puts("| <id do peao> (abcd, xyzw)          |");
    puts("| s - sair                           |");
    puts("| h - imprimir menu                  |");
    puts("+------------------------------------+");
}

/**
 * @brief Gets the board safe cells from the config file given as a program argument
 * @param fileName The name of the config file
 * @param safeCells An array which stores all the safe cells read from the config file
 * @return Returns 0 on 'SUCCESS' and 1 on 'FAILURE'
*/
int getSafeCellsFromConfigFile(char *fileName, int *safeCells) {
    FILE *fp;
    int numberRead;

    // Opens config file in 'read' mode
    fp = fopen(fileName, "r");

    // Checks for file read errors
    if (fp == NULL) {
        fputs(FILE_ERR1, stdout);
        return 1;
    }

    // Prints message upon opening the file to be read
    printf("fich %s\n", fileName);

    // Reads all numbers from config file and stores them in 'safeCells'
    while (!feof(fp)) {
        if (fscanf(fp, "%d", &numberRead) != 1) {
            // Prints ERROR message and breaks the loop in case the read value is not valid
            printf("%s", FILE_ERR2);
            break;
        }
        
        if (numberRead < MAX_CELLS) {
            safeCells[numberRead] = 1;
        }
    }

    // Closes file stream
    fclose(fp);
    return 0;
}
