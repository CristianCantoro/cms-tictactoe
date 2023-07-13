#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <vector>
#include "tictactoe.h"

#define NINPUT 2

using namespace std;

FILE *fin, *fout, *fifo_in, *fifo_out;

Move::Move() {
  row = 0;
  col = 0;
}

Move::Move(int r, int c) {
  row = r;
  col = c;
}

TicTacToe::TicTacToe() {
  // initialize the board with zeros
  for (auto& row : board) {
    for (auto& cell : row) {
      cell = 0;
    }
  }

  // set winner to -1
  //   * 0, draw
  //   * 1, player 1 wins
  //   * 2, player 2 wins
  winner = -1;
}

bool TicTacToe::has_ended() {
  if (winner == -1) {
    // winner is not set, check for a winning condition or a draw
    if (TicTacToe::check_winner(1)) {
      winner = 1;
      return true;
    }
    else if (check_winner(2)) {
      winner = 2;
      return true;
    }
    else if (is_draw()) {
      winner = 0;
      return true;
    }
  } else {
    // winner is set
    return true;
  }

  return false;
}

void TicTacToe::print_board(std::string prefix) const {
  char out = ' ';

  std::cout << prefix << "-------------" << std::endl;
  for (const auto& row : board) {
    std::cout << prefix << "| ";
    for (const auto& cell : row) {
      if (cell == 0) {
        out = ' ';
      } else if (cell == 1) {
        out = 'X';
      } else if (cell == 2) {
        out = 'O';
      }
      std::cout << out << " | ";
    }
    std::cout << std::endl << prefix << "-------------" << std::endl;
  }

  return;
}

int TicTacToe::get_winner() const {
  return winner;
}

std::vector<Move> TicTacToe::get_available_moves() const {
  std::vector<Move> moves;

  for (int i = 2; i >= 0; i--) {
    for (int j = 2; j >= 0; j--) {
      if (board[i][j] == 0) {
        moves.push_back(Move(i, j));
      }
    }
  }

  return moves;
}

void TicTacToe::move(int player, int row, int col) {
  if ((row < 0 || row > 2) || (col < 0 || col > 2)) {
    throw std::out_of_range("Move is out of bounds.");
  }

  if (board[row][col] != 0) {
    throw std::invalid_argument("Cell is already occupied.");
  }

  board[row][col] = player;

  return;
}

bool TicTacToe::check_winner(int player) const {
  // check rows
  for (const auto& row : board) {
    if (row[0] == player && row[1] == player && row[2] == player) {
      return true;
    }
  }

  // check columns
  for (int col = 0; col < 3; col++) {
  if (board[0][col] == player && board[1][col] == player && \
      board[2][col] == player) {
    return true;
  }
  }

  // Check diagonals
  if (board[0][0] == player && board[1][1] == player && \
      board[2][2] == player) {
    return true;
  }
  if (board[0][2] == player && board[1][1] == player && \
      board[2][0] == player) {
    return true;
  }

  return false;
}

bool TicTacToe::is_draw() const {
  for (const auto& row : board) {
    for (int cell : row) {
      if (cell == 0) {
        // found an empty cell, game is not a draw
        return false;
      }
    }
  }

  // all cells are filled, game is a draw
  return true;
}

void ex(const char *msg, float res) {
  if (msg) {
    fprintf(stderr, "%s ", msg);
  }

  // char resch[64];
  // sprintf(resch, "%f", res);
  // printf("%s\n", resch);
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
