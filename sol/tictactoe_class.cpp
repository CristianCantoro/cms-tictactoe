#ifndef TICTACTOE_CPP
#define TICTACTOE_CPP

#include <iostream>
#include <vector>
#include "tictactoe.h"

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

void TicTacToe::print_board() const {
  char out = ' ';

  std::cout << "-------------" << std::endl;
  for (const auto& row : board) {
    std::cout << "| ";
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
    std::cout << std::endl << "-------------" << std::endl;
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

#endif // TICTACTOE_CPP
