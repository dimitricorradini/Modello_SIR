#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

enum class State { Susc, Inf, Rec };

class Board {
  int n_;
  std::vector<State> board_;

public:
  Board(int n) : n_(n), board_(n * n) {}

  State operator()(int i, int j) const {
    return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j]
                                                  : State::Rec;
  }
  State& operator()(int i, int j) {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }
  int size() const { return n_; }
  
  void print() const {
    for (int l = 0; l <= n_; ++l) {
      std::cout << " =";
    }
    std::cout << " \n";
    for (int i = 0; i != n_; i++) {
      std::cout << "| ";
      for (int j = 0; j != n_; j++) {
        if (static_cast<State>(board_[i * n_ + j]) == State::Susc) {
          std::cout << "s ";
        }
        if (static_cast<State>(board_[i * n_ + j]) == State::Inf) {
          std::cout << "I ";
        }
        if (static_cast<State>(board_[i * n_ + j]) == State::Rec) {
          std::cout << "r ";
        }
      }
      std::cout << "|\n";
    }
    for (int l = 0; l <= n_; ++l) {
      std::cout << " =";
    }
    std::cout << '\n';
  }
};
/*
qui ho pensato di far leggere ogni cella della griglia con dei cicli for per vedere lo
stato di ogni cella, se è Susc o Rec non fa nulla, se è inf genera un numero random tra 
0 e 1000, se il numero è compeso tra 300 e 400(per avere una probabilità del 10%) allora 
cambia lo stato della cella in rec, altrimenti parte un altro ciclo for per controllare 
le celle che ha intorno, se ne trova una Susc e al contempo beta(il numero random) è 
compreso tra 0 e 300(prob del 30%) allora cambia quella cella (l,m)(dell'intorno di (i,j))
in una Inf sennò break.
*/
Board evolve(Board const &current) {
  int n = current.size();
  Board next(n);
  int r = 0;
  int c = 0;
  for (int i = 0; i != n; i++) {
    for (int j = 0; j != n; j++) {
      if (static_cast<State>(current(i, j)) == State::Susc &&
          static_cast<State>(current(i, j)) == State::Rec) {
        break;
      } else if (static_cast<State>(current(i, j)) == State::Inf) {
        float beta = rand() % 1000;
        if (beta >= 300 && beta < 400) {
          static_cast<State>(next(i, j)) == State::Rec;
        } else {
          for (int l = i - 1; l != i + 2; ++l) {
            for (int m = j - 1; m != j + 2; ++m) {
              if (static_cast<State>(current(l, m)) == State::Susc &&
                  beta < 300) {
                static_cast<State>(next(l, m)) == State::Inf;
              } else {
                break;
              }
            }
          }
        }
      }
    }
  }
  return next;
}

int main() {
  int dim = 10;
  Board board(dim);
  board(1, 1) = State::Inf;
  board.print();
  std::this_thread::sleep_for(std::chrono::milliseconds(900));
  for (int i = 0; i != 5; ++i) {
    std::cout << "\033c";
    board = evolve(board);
    board.print();
    std::this_thread::sleep_for(std::chrono::milliseconds(900));
  }
}
