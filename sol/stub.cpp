#include <iostream>
#include <cassert>
#include <iostream>
#include <vector>
#include "tictactoe.h"

#define NINPUT 1

using namespace std;

static FILE *fifo_in, *fifo_out;

Move player_move(TicTacToe &game);

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
      std::cout << "Player 1 (X) wins!" << std::endl;
      return true;
    }
    else if (check_winner(2)) {
      winner = 2;
      std::cout << "Player 2 (O) wins!" << std::endl;
      return true;
    }
    else if (is_draw()) {
      winner = 0;
      std::cout << "The game is a draw." << std::endl;
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
