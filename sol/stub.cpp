#include <iostream>
#include <cassert>
#include "tictactoe.h"

#define NINPUT 1

using namespace std;

static FILE *fifo_in, *fifo_out;

Move player_move(TicTacToe &game);

Move computer_move() {
  Move move;
  assert(fscanf(fifo_in, "%d %d", &(move.row), &(move.col)) == 2);
  printf("[stub]: computer move: (%d, %d)\n", move.row, move.col);

  return move;
}

int main(int argc, char **argv) {
  fifo_in = fopen(argv[2], "r");
  fifo_out = fopen(argv[1], "w");

  if (fifo_in == NULL) {
    fprintf(stderr, "Failed to open input queue.\n");
    return 1;
  }

  if (fifo_out == NULL) {
    fprintf(stderr, "Failed to open output queue.\n");
    return 1;
  }

  // read input
  int player, computer;
  bool player_first = false;
  assert(fscanf(fifo_in, "%d", &player) == NINPUT);
  if (player == 1) {
    computer = 2;
    player_first = true;
  } else {
    computer = 1;
    player_first = false;
  }
  // printf("player: %d, computer: %d, player, computer);

  TicTacToe game;
  Move move;
  int turn = 0;

  while (!game.has_ended()) {
    // increase turn count
    turn++;
    printf("[stub]: --> turn %d\n", turn);

    if (turn % 2 == (player_first ? 1 : 0)) {
      // player's turn
      move = player_move(game);
      printf("[stub]: player (player %d) move: (%d, %d)\n", player,
             move.row, move.col);

      fprintf(fifo_out, "%d %d\n", move.row, move.col);
      fflush(fifo_out);

      try {
        game.move(player, move.row, move.col);
      }
      catch (const out_of_range& e) {
        fprintf(stderr, "[stub]: %s", e.what());
        return 1;
      }
      catch (const invalid_argument& e) {
        fprintf(stderr, "[stub]: %s", e.what());
        return 1;
      }
    } else {
      // computer's turn

      move = computer_move();
      printf("[stub]: computer (player %d) move: (%d, %d)\n", computer,
             move.row, move.col);

      try {
        game.move(computer, move.row, move.col);
      }
      catch (const out_of_range& e) {
        fprintf(stderr, "[stub]: %s\n", e.what());
        return 1;
      }
      catch (const invalid_argument& e) {
        fprintf(stderr, "[stub]: %s\n", e.what());
        return 1;
      }
    }

    game.print_board("[stub]: ");
  }

  fclose(fifo_in);
  fclose(fifo_out);

  return 0;
}
