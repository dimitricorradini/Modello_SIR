#include "board.hpp"
#include <iostream>
#include <random>

SIR::Board SIR::evolve(SIR::Board const &current, double const beta,
                       double const gamma, double const mu) {
  if (beta > 1 || gamma > 1 || mu > 1 || beta < 0 || gamma < 0 || mu < 0) {
    throw std::runtime_error{
        "The coefficients beta, gamma and mu must be between 0 and 1."};
  }

  int n = current.side();
  Board next(n);

  // random generation
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist{0, 1};

  // cycle on board
  for (int i = 0; i != n; ++i) {
    for (int j = 0; j != n; ++j) {
      if (current(i, j) == State::Inf) {
        double prob_rec = dist(gen);
        if (prob_rec < gamma) {
          next(i, j) = State::Rec;
        } else {
          double prob_death = dist(gen);
          if (prob_death < mu) {
            next(i, j) = State::Dead;
          } else {
            next(i, j) = State::Inf;
            // cycle on neighbors
            for (int l = i - 1; l <= i + 1; ++l) {
              for (int m = j - 1; m <= j + 1; ++m) {
                double prob_inf = dist(gen);
                if (current(l, m) == State::Susc && prob_inf < beta) {
                  next(l, m) = State::Inf;
                }
              }
            }
          }
        }
      } else {
        if (next(i, j) != State::Inf) {
          next(i, j) = current(i, j);
        }
      }
    }
  }
  return next;
}
