#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <sstream>
#include "tictactoe.h"

#define NINPUT 2

using namespace std;

FILE *fin, *fout, *fifo_in, *fifo_out;

void ex(const char *msg, float res) {
  if (msg) {
    fprintf(stderr, "%s ", msg);
  }
  printf("%f\n", res);
  exit(0);
}

Move player_move() {
  Move move;
  assert(fscanf(fifo_out, "%d %d", &(move.row), &(move.col)) == 2);

  return move;
}

// random player
Move computer_move(TicTacToe &game) {
  vector<Move> moves = game.get_available_moves();

  int r = rand() % moves.size();
  Move move = moves[r];

  return move;
}

int main(int argc, char **argv) {
  signal(SIGPIPE, SIG_IGN);

  fin = fopen("input.txt", "r");
  fout = fopen("output.txt", "w");
  fifo_in = fopen(argv[1], "w");
  fifo_out = fopen(argv[2], "r");

  if (fifo_in == NULL) {
    fprintf(stderr, "Failed to open input queue.\n");
    return 1;
  }

  if (fifo_out == NULL) {
    fprintf(stderr, "Failed to open output queue.\n");
    return 1;
  }

  int random_seed;
  int player, computer;
  bool player_first = false;

  // read input
  assert(fscanf(fin, "%d %d", &player, &random_seed) == NINPUT);

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

  fprintf(fifo_in, "%d\n", player);
  fflush(fifo_in);

  TicTacToe game;
  Move move;
  int turn = 0;

  // game has not ended
  while (!game.has_ended()) {
    // increase turn count
    turn++;

    if (turn % 2 == (player_first ? 1 : 0)) {
      // player's turn, read its move
      move = player_move();

      try {
        game.move(player, move.row, move.col);
      }
      catch (const out_of_range& e) {
        ex(e.what(), 0.0);
      }
      catch (const invalid_argument& e) {
        ex(e.what(), 0.0);
      }

      fprintf(fout, "%d %d %d\n", player, move.row, move.col);
      fflush(fout);
    } else {
      // computer's turn
      move = computer_move(game);
      game.move(computer, move.row, move.col);

      fprintf(fout, "%d %d %d\n", computer, move.row, move.col);
      fflush(fout);
      fprintf(fifo_in, "%d %d\n", move.row, move.col);
      fflush(fifo_in);
    }
  }

  // game has ended
  int winner = game.get_winner();
  char msg[128];

  if (winner == 0) {
    // game is a draw
    ex("game is a draw!", 0.5);
  } 
  if (winner == player) {
    // player wins
    sprintf(msg, "player (player %d) wins!", player);
    ex(msg, 1.0);
  } else {
    // computer wins
    sprintf(msg, "player (player %d) lose!", player);
    ex(msg, 0.0);
  }

  fclose(fin);
  fclose(fout);
  fclose(fifo_in);
  fclose(fifo_out);

  return 0;
}
