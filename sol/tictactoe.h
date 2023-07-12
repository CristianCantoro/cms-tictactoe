#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <array>
#include <vector>

class Move {

public:
  int row;
  int col;

  Move();
  Move(int r, int c);
};

class TicTacToe {
private:
  std::array<std::array<int, 3>, 3> board;
  int winner;

public:
  TicTacToe();

  void print_board(std::string prefix) const;
  int get_winner() const;
  std::vector<Move> get_available_moves() const;

  bool has_ended();
  void move(int player, int row, int col);

private:
  bool check_winner(int player) const;
  bool is_draw() const;
};

#endif // TICTACTOE_H
