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

/*
// random player
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
*/