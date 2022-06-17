#include <stdio.h>
#include <stdlib.h>

#include "board.h"


#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

/* Program Functions' Signature */

int getSafeCellsFromConfigFile(char *fileName, int *safeCells);
void initializeCellsList(list *boardCells);
int insertBoardCell(list *boardCells, node cell);
node cell;


int main(int argc, char const *argv[])
{
    /* Program variables */
    // Default values for 'Board Presentation Mode', 'Number of Lines' and 'Number of Columns'
    int boardPresentation = 0, linesNum = 3, columnsNum = 7;
    int safeCells[MAX_CELLS] = {0};  // Stores the safe cells read from the config file
    int totalCells;  // Number of total cells
    list boardCells;  // List struct to store all board cells data

    // Initializes random seed
    srand(1);


    // Gets program args and checks if they're valid
    for (int i = 1; i < argc; i++) {
        // Checks if 'Board Presentation Mode' argument is set and valid
        if (i == 1 && (atoi(argv[i]) == 0 || atoi(argv[i]) == 1)) {
            boardPresentation = atoi(argv[i]);
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

    for (int cell = 0; cell < totalCells; cell++) {
        printf("Number in pos %d: %d\n", cell, safeCells[cell]);
    }
    
    
    return 0;
}


/* Program Functions */

/**
 * @brief Gets the board safe cells from the config file given as a program argument
 * @param fileName The name of the config file
 * @param safeCells An array which stores all the safe cells read from the config file
 * @return Returns 0 on 'SUCCESS' and 1 on 'FAILURE'
*/
int getSafeCellsFromConfigFile(char *fileName, int *safeCells) {
    FILE *fp;
    int cellIndex = 0;
    int numberRead;

    // Opens config file in 'read' mode
    fp = fopen(fileName, "r");

    // Checks for file read errors
    if (fp == NULL) {
        fputs(FILE_ERR1, stdout);
        return 1;
    }

    // Reads all numbers from config file and stores them in 'safeCells'
    while (fscanf(fp, "%d", &numberRead) != EOF && cellIndex < MAX_CELLS) {
        safeCells[cellIndex] = numberRead;
        cellIndex++;
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
int insertBoardCell(list *boardCells, node cell) {
    // Creates a new node
    node *boardCell = (node*) malloc(sizeof(node));

    // Checks if an ERROR ocurred (i.e. out of memory)
    if (boardCell == NULL) return 1;

    // Adds cell data from the given cell to the new cell pointer
    boardCell->item = cell.item;
    boardCell->next = NULL;
    
    if (boardCells->head == NULL) {  // List is empty
        boardCells->head = boardCell;
        boardCells->tail = boardCell;
        boardCells->length = 1;
    } else {  // List has more than 1 item
        boardCells->tail->next = boardCell;
        boardCells->tail = boardCell;
        boardCells->length++;
    }

    return 0;
}