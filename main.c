#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"


#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

/* Program Functions' Declaration */

void showMenu();
int getSafeCellsFromConfigFile(char *fileName, int *safeCells);
void initializeCellsList(list *boardCells);
int insertBoardCell(list *boardCells, node *cell);
int boardSetup(list *boardCells, int *safeCells, int totalCells);


int main(int argc, char const *argv[])
{
    /* Program variables */
    // Default values for 'Board Presentation Mode', 'Number of Lines' and 'Number of Columns'
    unsigned int boardPresentationMode = 0, linesNum = 3, columnsNum = 7;
    int safeCells[MAX_CELLS] = {0};  // Stores the safe cells read from the config file
    unsigned int totalCells;  // Number of total cells
    list boardCells;  // List struct to store all board cells data

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
    
    // Prints board
    boardPrint(linesNum, columnsNum, boardCells, boardPresentationMode);

    // Game Loop
    /*do {

    } while ();
    */
    
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

    // Reads all numbers from config file and stores them in 'safeCells'
    while (fscanf(fp, "%d", &numberRead) != EOF && numberRead < MAX_CELLS) {
        safeCells[numberRead] = 1;
    }

    // Closes file stream
    fclose(fp);
    return 0;
}

/**
 * @brief Initializes the board cells' list
 * @param boardCells The list that holds the board cells
 */
void initializeCellsList(list *boardCells) {
    boardCells->head = NULL;
    boardCells->tail = NULL;
    boardCells->length = 0;
}

/**
 * @brief Inserts a new board cell into the board cells' linked list.
 * @param boardCells The board cells list
 * @param cell The cell to be inserted
 * @return Returns 0 on 'SUCCESS' and 1 on 'FAILURE'
 */
int insertBoardCell(list *boardCells, node *cell) {
    if (boardCells->head == NULL) {  // List is empty
        boardCells->head = cell;
        boardCells->tail = cell;
        boardCells->length = 1;
    } else {  // List has more than 1 item
        boardCells->tail->next = cell;
        boardCells->tail = cell;
        boardCells->length++;
    }

    return 0;
}

/**
 * @brief Performs board setup. Initializes all board cells 
 * and places home cells as well as safe cells.
 * @param boardCells Linked list with board cells
 * @param safeCells Int array with safe cells position
 * @param totalCells The number of total cells
 * @return Returns 0 on 'SUCCESS' and 1 on 'FAILURE'
 */
int boardSetup(list *boardCells, int *safeCells, int totalCells) {
    // Adds cells to board (Board Setup)
    for (int cellIndex = 0; cellIndex < totalCells; cellIndex++) {
        int letterIdx;
        node *cell = (node*) malloc(sizeof(node));  // Creates a new 'node'
        casa *place = (casa*) malloc(sizeof(casa));  // Creates a new 'casa' for the node

        // Checks if an ERROR ocurred while allocating memory (i.e. out of memory)
        if (cell == NULL || place == NULL) {
            return 1;
        }

        // Adds data to new place
        if (cellIndex == 0) {  // Initializes home cell for player 1
            for (letterIdx = 0; letterIdx <= 3; letterIdx++) {
                place->jogador_peao[0][letterIdx] = 1;
                place->jogador_peao[1][letterIdx] = 0;
            }

            // Sets home cells as safe cells
            place->casaSegura = 1;

        } else if (cellIndex == (totalCells / 2)) {  // Initializes home cell for player 2
            for (letterIdx = 0; letterIdx <= 3; letterIdx++) {
                place->jogador_peao[1][letterIdx] = 1;
                place->jogador_peao[0][letterIdx] = 0;
            }

            // Sets home cells as safe cells
            place->casaSegura = 1;
        } else {
            // Initializes safe cells given in the config file
            if (safeCells[cellIndex] == 1) {
                place->casaSegura = 1;
            } else {
                place->casaSegura = 0;
            }

            // Cleans all the other player cells
            for (letterIdx = 0; letterIdx <= 3; letterIdx++) {
                place->jogador_peao[0][letterIdx] = 0;
                place->jogador_peao[1][letterIdx] = 0;
            }
        }

        // Adds data to new cell
        cell->item = *place;
        cell->next = NULL;
        
        // Insert new cell onto the board
        if (insertBoardCell(boardCells, cell) == 1) {
            return 1;
        }

        // Frees memory from 'place'
        free(place);
    }

    return 0;
}