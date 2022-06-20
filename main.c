#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "board.h"


#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

/* Program Functions' Declaration */

void showMenu();
int getSafeCellsFromConfigFile(char *fileName, int *safeCells);
void initializeCellsList(list *boardCells);
int insertBoardCell(list *boardCells, node *cell);
int boardSetup(list *boardCells, int *safeCells, int totalCells);
int checkGameWin(list *boardCells, int totalCells);
int getPawnNodeIndex(list *boardCells, char pawn);
void movePawn(list *boardCells, char pawn, int pawnIndex, int srcIndex, int destIndex);
void makePlay(list *boardCells, char pawn, int amount);


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
                // Skips
                break;
            
            default:
                // TODO
            
                break;
        }

        // Changes player move
        player1 = !player1;
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

/**
 * @brief Checks if any of the two player has already won the game.
 * @param boardCells Linked list with board cells
 * @param totalCells The number of total cells
 * @return Returns 1 if 'Player 1 WON the game', 2 if 'Player 2 WON the game' and 0 if the 'Game still in progress'
 */
int checkGameWin(list *boardCells, int totalCells) {
    int homeP1 = 0;  // Player 1 home
    int homeP2 = totalCells / 2;  // Player 2 home
    bool p1Won;  // Player 1 win case
    bool p2Won;  // Player 2 win case

    node currentNode = *boardCells->head;  // Holds current node being checked

    for (int i = homeP1; i <= homeP2; i++) {
        if (i == homeP1) {
            int playerPawn[4] = currentNode.item.jogador_peao[0];
            p1Won = playerPawn[0] == 'A' && playerPawn[1] == 'B' && playerPawn[2] == 'C' && playerPawn[3] == 'D';
        }

        if (i == homeP2) {
            int playerPawn[4] = currentNode.item.jogador_peao[1];
            p2Won = playerPawn[0] == 'W' && playerPawn[1] == 'X' && playerPawn[2] == 'Y' && playerPawn[3] == 'Z';
        }
        
        // Gets next node
        currentNode = *currentNode.next;
    }

    // Checks which player won, if any
    if (p1Won) {
        return 1;
    } else if (p2Won) {
        return 2;
    } else {
        return 0;
    }
}

/**
 * @brief Get the Node Index for the given pawn.
 * @param boardCells Linked list with board cells
 * @param pawn The given pawn
 * @return Returns the node index for the given pawn
 */
int getPawnNodeIndex(list *boardCells, char pawn) {
    int playerPos;  // Gets the player position to which the pawn belongs (0 - P1, 1 - P2)
    int pawnPos;  // Gets the pawn position
    int nodeIndex = 0;  // Holds the current node index being checked 
    
    // Sets 'plaerPos' and 'pawnPos' based on the given 'pawn'
    switch (pawn) {
        case 'a':
            playerPos = 0;
            pawnPos = 0;
            break;
        case 'b':
            playerPos = 0;
            pawnPos = 1;
            break;
        case 'c':
            playerPos = 0;
            pawnPos = 2;
            break;
        case 'd':
            playerPos = 0;
            pawnPos = 3;
            break;
        case 'w':
            playerPos = 1;
            pawnPos = 0;
            break;
        case 'x':
            playerPos = 1;
            pawnPos = 1;
            break;
        case 'y':
            playerPos = 1;
            pawnPos = 2;
            break;
        case 'z':
            playerPos = 1;
            pawnPos = 3;
            break;
        
        default:
            break;
    }
    
    // Iterates over the board cells until it finds the pawn position
    for (node currentNode = *boardCells->head; currentNode.next != NULL; currentNode = *currentNode.next) {
        if ((char) currentNode.item.jogador_peao[playerPos][pawnPos] == pawn) {
            return nodeIndex;
        }
        nodeIndex++;
    }

    return -1;
}

/**
 * @brief Moves a pawn from a given source to a given destination.
 * @param boardCells Linked list with board cells
 * @param pawn The given pawn
 * @param pawnIndex The index in 'jogador_peao' array inside 'node' for the current pawn
 * @param srcIndex The index for the node where the pawn currently resides
 * @param destIndex The index for the node to which the pawn should reside from now on
 */
void movePawn(list *boardCells, char pawn, int pawnIndex, int srcIndex, int destIndex) {
    int playerIndex;
    int currentIndex = 0;

    // Gets player index to access based on the given pawn
    if (pawn == 'a' || pawn == 'b' || pawn == 'c' || pawn == 'd') {
        playerIndex = 0;
    } else {
        playerIndex = 1;
    }
    
    // TODO: fix this

    for (node *currentNode = boardCells->head; currentNode->next != NULL; currentNode = currentNode->next) {
        if (currentIndex == srcIndex) {
            // Removes the pawn from its current position in the board
            currentNode->item.jogador_peao[playerIndex][pawnIndex] == FALSE;
        }

        // Checks if pawn has gone around the whole board, if that's the case, the pawn must be uppercase
        if (destIndex >= boardCells->length) {
            currentNode->item.jogador_peao[playerIndex][pawnIndex] = WIN;
        }

        // Places the pawn in its new destination
        if (currentIndex == destIndex) {
            currentNode->item.jogador_peao[playerIndex][pawnIndex] = TRUE;
        }

        currentIndex++;
    }
}

void makePlay(list *boardCells, char pawn, int amount) {
    int playerIndex;
    int homeOpponent;
    int currentIndex = 0;  // Stores current node index
    int pawnIndex;  // Stores the index of the pawn in the cell
    char playerSymbols[10];  // Stores string with player symbols

    // Gets current pawn node index
    int pawnCurrentPos = getPawnNodeIndex(boardCells, pawn);

    // Gets player index based on pawn, sets 'playerSymbols' based on it, as well as 'homeOpponent'
    if (pawn == 'a' || pawn == 'b' || pawn == 'c' || pawn == 'd') {
        playerIndex = 0;
        strcpy(playerSymbols, SYMBOLS_J1);
        homeOpponent = boardCells->length / 2;
    } else {
        playerIndex = 1;
        strcpy(playerSymbols, SYMBOLS_J2);
        homeOpponent = 0;
    }

    // Gets pawn index in the cell
    for (pawnIndex = 1; pawnIndex < 5; pawnIndex++) {
        if (SYMBOLS_J1[pawnIndex] == pawn || SYMBOLS_J2[pawnIndex] == pawn) {
            break;
        }
    }

    // Moves the chosen 'pawn' to its destination based on 'amount' (dices value)
    movePawn(boardCells, pawn, pawnIndex, pawnCurrentPos, pawnCurrentPos + amount);

    /* 
        Checks every board cell that the current pawn will go through.
        If the cell is not a safe cell, moves all the other player
        cell to his home cell.
    */
    for (node *currentNode = boardCells->head; currentNode->next != NULL; currentNode = currentNode->next) {
        if (currentIndex > pawnCurrentPos && currentIndex <= pawnCurrentPos + amount) {
            // Checks if the current node has any of the opponnent players pawns
            for (int symbol = 1; symbol < 5; symbol++) {
                if (currentNode->item.jogador_peao[playerIndex][symbol-1] == playerSymbols[symbol]) {
                    movePawn(boardCells, playerSymbols[symbol], symbol-1, currentIndex, homeOpponent);
                }
            }
        }
    }
}