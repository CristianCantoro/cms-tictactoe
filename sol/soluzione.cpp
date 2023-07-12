#include <iostream>
#include <vector>
#include "tictactoe.h"

using namespace std;

const char* getNextLineFromLiteralString(const char* literalString) {
    static const char* currentPos = literalString;
    
    if (*currentPos == '\0') {
        return nullptr; // End of string
    }
    
    const char* lineStart = currentPos;
    
    // Find the end of the line
    while (*currentPos != '\0' && *currentPos != '\n') {
        currentPos++;
    }
    
    if (*currentPos == '\n') {
        currentPos++; // Move past the newline character
    }
    
    return lineStart;
}

/*
 * (0, 0) position 1
 * (0, 1) position 2
 * (0, 2) position 3
**/

static const char *mymoves = R"""(0 0
0 1
0 2
)""";

/*
Move player_move(TicTacToe &game) {
	Move move;

	const char* line = getNextLineFromLiteralString(mymoves);
  sscanf(line, "%d %d\n",  &(move.row), &(move.col));
  // printf("%d %d\n", move.row, move.col);

  return move;
}
*/

Move player_move(TicTacToe &game) {
  vector<Move> moves = game.get_available_moves();
 
 	printf("n. of available moves: %d\n", (int) moves.size());
  for (auto move : moves) {
  	printf("- (%d, %d)\n", move.row, move.col);
  }

  int r = rand() % moves.size();
  Move move = moves[r];
  printf("player move\n");

  return move;
}