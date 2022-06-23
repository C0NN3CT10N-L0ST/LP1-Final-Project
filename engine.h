#ifndef __engine_h__
#define __engine_h__

#include "board.h"

#define MAX_CELLS 128  // Defines the max number of cells that can exist in the board

void initializeCellsList(list *boardCells);
int insertBoardCell(list *boardCells, node *cell);
int boardSetup(list *boardCells, int *safeCells, int totalCells);
bool validPawn(char pawn, bool player1);
int checkGameWin(list *boardCells, int totalCells);
int getPawnIndex(char pawn);
int getPawnNodeIndex(list *boardCells, char pawn);
void movePawn(list *boardCells, char pawn, int pawnIndex, int srcIndex, int destIndex);
void resetAdversaryPawn(list *boardCells, char pawn, int player, int pawnSrcIndex);
void makePlay(list *boardCells, char pawn, int amount);
bool pawnCompletesLapInCurrentPlay(int player, int totalCells, int srcIndex, int destIndex, int finalDestIndex);
bool isPawnMovable(char pawn, list *boardCells, bool player);

#endif