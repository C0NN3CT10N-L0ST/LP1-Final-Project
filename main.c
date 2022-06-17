#include <stdio.h>
#include "board.h"


#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

/* Program Functions' Signature */
void getSafeCellsFromConfigFile(char *fileName, int *safeCells);


int main(int argc, char const *argv[])
{
    /* Program variables */
    // Default values for 'Board Presentation Mode', 'Number of Lines' and 'Number of Columns'
    int boardPresentation = 0, linesNum = 3, columnsNum = 7;
    int safeCells[MAX_CELLS];  // Stores the safe cells read from the config file
    
    
    // Gets program args and checks if they're valid
    for (int i = 1; i < argc; i++) {
        // Checks if 'Board Presentation Mode' argument is set and valid
        if (i == 1 && (argv[i] == 0 || argv[i] == 1)) {
            boardPresentation = argv[i];
        }

        // Checks if the 'Number of lines' is set and valid
        if (i == 2 && argv[i] >= MIN_ROWS && (int)argv[i] % 2 != 0) {
            linesNum = argv[i];
        }

        // Checks if the 'Number of Columns' is set and valid
        if (i == 3 && argv[i] > MIN_COLS) {
            columnsNum = argv[i];
        }

        // Checks if the 'Configuration File' is present and reads its content
        if (i == 4) {
            // Reads safe cells from config file and stores them in an array
            getSafeCellsFromConfigFile(argv[i], &safeCells);
        }
    }
    
    
    return 0;
}


/* Program Functions */
void getSafeCellsFromConfigFile(char *fileName, int *safeCells) {
    FILE *fp;
    int cellIndex = 0;
    int numberRead;

    // Opens config file in 'read' mode
    fp = fopen(fileName, "r");

    // Checks for file read errors
    if (fp == NULL) {
        fputs(stdout, FILE_ERR1);
        exit(0);
    }

    // Reads all numbers from config file and stores them in 'safeCells'
    while (fscanf(fp, "%d", &numberRead) != EOF && cellIndex < MAX_CELLS) {
        safeCells[cellIndex] = numberRead;
        cellIndex++;
    }

    // Closes file stream
    fclose(fp);
}