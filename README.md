# LP1 Final Project - Don't Catch a Cold

## Game Description
***Don't Catch a Cold*** is a two-player board game. Each of the players will have 4 pawns:
* Player 1 - `a` `b` `c` `d`
* Player 2 - `w` `x` `y` `z`

At the beginning all 4 pawns from each player will be in their respective home cell. In each play, the player will rolls
the dices and chooses a pawn to be moved to the next cell (calculated based on the dices result).</br>
The game finishes when all pawns from one of the two players complete a full lap around the board (when the pawn reaches
its home again).</br>
In each play, when a pawn from a player overtakes a pawn from the adversary and the board cell the adversary pawn is on is not a *Safe
Cell* (explained below) the adversary pawn will *Catch a Cold* and will be returned to its home cell. After a pawn is
returned to its home cell, it is able to be used again.</br>
Some of the board cells are *Safe Cells*, which means that when pawns are on that cell they won't *Catch a Cold* in case
they're overtaken by an opponent pawn (as a result they don't need to return to their home cell).

### The purpose of this game is to practice the following topics:
* Variables
* Operators
* Conditions
* Vectors and/or matrices
* Strings
* Loops
* Structs
* Linked lists
* File handling
* CLI arguments handling
* Dynamic memory
### The game should take into account the following aspects:
* The code must be implemented using the C programming language
* The code must compile without any *errors or warnings* using the following ***gcc*** flags:</br>
`-g -Wvla -Wall -Wpedantic -Wextra -Wdeclaration-after-statement`
* Variable names' should be indicative of what they'll contain
* Global variables are not allowed
* The use of `strtok()` function and `goto` instruction is not allowed

## Game Board
The game board has a rectangular/squared shape and the number of lines as well as the number of columns can be given
using the CLI arguments when running the game.</br>
If no CLI args are given, the board must have the following default values:
* **Number of lines:** 3
* **Number of columns:** 7

## Game Rules
* Each play starts by rolling two dices. The result is a value between 2 and 12.
* After rolling the dices, the current player must choose which pawn he wants to move. The selected pawn will the
  advance clockwise the amount of cells given by the dices result.
* If the pawn goes through a cell where there is an adversary pawn, the opponent pawn will return to its home cell, i.e.
  the pawn *Catches a Cold*.
* If the pawn completes a full board lap in the current play, then:
    * The pawn returns to its home cell
    * The pawn can't be played again
    * The pawn is represented in its home cell as uppercase (to signal it has already completed a full lap)

##