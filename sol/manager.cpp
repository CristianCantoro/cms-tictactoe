#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "tictactoe.h"

using namespace std;

FILE *fin, *fout, *fifo_in, *fifo_out;

Move player_move() {
  Move move;
  assert(fscanf(fifo_out, "%d %d", &(move.row), &(move.col)) == 2);
  printf("player move: (%d, %d)\n", move.row, move.col);

  return move;
}

Move computer_move(TicTacToe &game) {
  vector<Move> moves = game.get_available_moves();

  printf("n. of available moves: %d\n", (int) moves.size());
  for (auto move : moves) {
    printf("- (%d, %d)\n", move.row, move.col);
  }

  int r = rand() % moves.size();
  Move move = moves[r];
  printf("computer move\n");

  return move;
}


int main(int argc, char **argv) {
  signal(SIGPIPE, SIG_IGN);

  fin = fopen("input.txt", "r");
  fout = fopen("output.txt", "w");
  fifo_in = fopen(argv[1], "w");
  fifo_out = fopen(argv[2], "r");

  if (fifo_in == NULL) {
    cerr << "Failed to open input queue." << endl;
    return 1;
  }

  if (fifo_out == NULL) {
    cerr << "Failed to open output queue." << endl;
    return 1;
  }

  int random_seed;
  int player, computer;
  bool player_first = false;

  // read input
  fscanf(fin, "%d %d", &player, &random_seed);

  // set random seed
  srand(random_seed);

  // set-up players
  if (player == 1) {
    computer = 2;
    player_first = true;
  } else {
    computer = 1;
    player_first = false;
  }
  printf("player: %d, computer: %d random_seed: %d\n",
         player, computer, random_seed);
  fflush(stdout);

  fprintf(fifo_in, "%d\n", player);
  fflush(fifo_in);

  TicTacToe game;
  Move move;
  int turn = 0;

  while (!game.has_ended()) {
    // increase turn count
    turn++;
    printf("--> turn %d\n", turn);

    if (turn % 2 == (player_first ? 1 : 0)) {
      // player's turn, read its move
      move = player_move();
      printf("player (player %d) move: (%d, %d)\n", player, move.row, move.col);
      game.move(player, move.row, move.col);
    } else {
      // computer's turn
      move = computer_move(game);
      printf("computer (player %d) move: (%d, %d)\n", computer, move.row, move.col);
      game.move(computer, move.row, move.col);
      fprintf(fifo_in, "%d %d\n", move.row, move.col);
      fflush(fifo_in);
    }

    game.print_board();
  }


  /*
  if (a+b == res) {
    fprintf(stderr, "A-ha, you're the best adding program I've ever met!\n");
    printf("1.0\n");
  } else {
    fprintf(stderr, "How dreadful, never met anyone as dumb as you...\n");
    printf("0.0\n");
  }
  */

  fclose(fin);
  fclose(fout);
  fclose(fifo_in);
  fclose(fifo_out);

}

