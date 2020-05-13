#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <random>

enum class State { Susc, Inf, Rec };

class Board {
  int n_;
  int i_stopper;
  //weird way to avoid making you wait
  std::vector<State> board_;

public:
  Board(int n) : n_(n), board_(n * n){}

  State operator()(int i, int j) const {
    return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j] : State::Rec;
  }

  State& operator()(int i, int j) {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }

  int size() const { return n_; }

  int infnum() const { return i_stopper; }
  
  void print1()
  {
    int I = 0;
    int R = 0;

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
          I++;
        }
        if (board_[i * n_ + j] == State::Rec) {
          std::cout << "r ";
          R++;
        }
      }
      std::cout << "|\n";
    }
    for (int l = 0; l <= n_; ++l) {
      std::cout << " =";
    }
    std::cout << "\n";

    std::cout << "Susceptible = " << (n_*n_)-I-R << "\n";
    std::cout << "Infected = " << I << "\n";
    std::cout << "Recovered = "<< R << "\n";

    i_stopper = I;
  }
};


Board evolve(Board const& current, double const beta, double const gamma) {
  //add exception for value of beta, gamma
  if (beta>1||gamma>1||beta<0||gamma<0) {
    throw std::runtime_error("Coefficients Beta and Gamma must be between 0 and 1"); 
  }

  int n = current.size();
  Board next(n);
  //seed for random generation
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist{0, 1};
  //cycle on matrix
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != n; ++j) {
      if (current(i, j) == State::Inf) {
        double prob1 = dist(gen);
        if (prob1 < gamma) {
          //recovery/death
          next(i, j) = State::Rec;
        } else {
          next(i, j) = State::Inf;
          //cycle on neighbors
          for (int l = i - 1; l <= i + 1; ++l) {
            for (int m = j - 1; m <= j + 1; ++m) {
              double prob2 = dist(gen);
              if (current(l, m) == State::Susc && prob2 < beta) {
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
  while (true) {
    std::cout << "\033c";
    board = evolve(board, 0.9, 0.2);
    board.print1();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (board.infnum()==0){
      std::cout<<"The virus is gone\n";
      return 0;
    }
  }
}
