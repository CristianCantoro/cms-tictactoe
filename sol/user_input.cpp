#include <iostream>
#include <vector>
#include "tictactoe.h"

using namespace std;

// ask the user on stdin
Move player_move(TicTacToe &game) {
  Move move;

  cout << "Move: ";
  cin >> move.row >> move.col;
  printf("player move: (%d, %d)\n", move.row, move.col);

  return move;
}
