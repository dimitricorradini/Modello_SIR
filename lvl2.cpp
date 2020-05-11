#include <cassert>
#include <chrono>
#include <ctime>
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
  
  
  void print1() const {//could be improved(?)
    int I = 0
    int R = 0
    for (int l = 0; l <= n_; ++l) {
      std::cout << " =";
    }
    std::cout << " \n";
    for (int i = 0; i != n_; i++) {
      std::cout << "| ";
      for (int j = 0; j != n_; j++) {
        if (board_[i * n_ + j] == State::Susc) {
          std::cout << "s ";
        }
        if (board_[i * n_ + j] == State::Inf) {
          std::cout << "I ";
          I++
        }
        if (board_[i * n_ + j] == State::Rec) {
          std::cout << "r ";
          R++
        }
      }
      std::cout << "|\n";
    }
    for (int l = 0; l <= n_; ++l) {
      std::cout << " =";
    }
    std::cout << "\n";
    std::cout << "Susceptible = " << n_-I-R;
    std::cout << "Infected = " << I;
    std::cout << "Recovered = "<< R;
    //a stopper
    if (I==0) {return 0;}
  }
};


Board evolve(Board const &current, double const& beta, double const& gamma) {
  //add exception for value of beta, gamma
  if (Beta>1||Gamma>1||Beta<0||Gamma<0) {
    throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"}; 
  };
  int n = current.size();
  int I =
  Board next(n);
  //seed for random generation
  srand(time(NULL));
  //cycle on matrix
  for (int i = 0; i != n; i++) {
    for (int j = 0; j != n; j++) {
      if (current(i, j) == State::Inf) {
        int prob1 = rand() % 1000;
        if (prob1 < (gamma*1000)) {
          //recovery/death
          next(i, j) = State::Rec;
        } else {
          next(i, j) = State::Inf;
          //cycle on neighbors
          for (int l = i - 1; l != i + 2; ++l) {
            for (int m = j - 1; m != j + 2; ++m) {
              int prob2 = rand()%1000;
              if (current(l, m) == State::Susc && prob2 < beta*1000) {
                //infection!
                next(l, m) = State::Inf;
              }
            }
          }
        }
      } else {
        if (next(i,j)!=State::Inf){//non può disinfettarsi in automatico!
          next(i,j)=current(i,j);
        }
      }
    }
  }
  return next;
}

int main() {
  int dim = 20;
  Board board(dim);
  board(1, 1) = State::Inf;
  board(1, 2) = State::Inf;
  board.print1();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  for (int i = 0; i != 100; ++i) {
    std::cout << "\033c";
    board = evolve(board, 0.3, 0.4);
    board.print1();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
