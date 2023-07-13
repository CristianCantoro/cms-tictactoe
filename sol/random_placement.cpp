#include <iostream>
#include <vector>
#include "tictactoe.h"

using namespace std;

// random player
Move player_move(TicTacToe &game) {
  vector<Move> moves = game.get_available_moves();

  srand(111);
  int r = rand() % moves.size();
  Move move = moves[r];
  printf("player move\n");

  return move;
}
