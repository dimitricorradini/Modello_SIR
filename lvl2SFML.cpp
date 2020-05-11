#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
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
  State &operator()(int i, int j) {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }
  // bool operator!() {return !board_(n_);}
  int size() const { return n_; }
};

Board evolve(Board const &current, double const &beta, double const &gamma) {
  // add exception for value of beta, gamma
  if (beta > 1 || gamma > 1 || beta < 0 || gamma < 0) {
    throw std::runtime_error{
        "Coefficients Beta and Gamma must be between 0 and 1"};
  };
  int n = current.size();
  Board next(n);
  // seed for random generation
  srand(time(NULL));
  // cycle on matrix
  for (int i = 0; i != n; i++) {
    for (int j = 0; j != n; j++) {
      if (current(i, j) == State::Inf) {
        int prob1 = rand() % 1000;
        if (prob1 < (gamma * 1000)) {
          // recovery/death
          next(i, j) = State::Rec;
        } else {
          next(i, j) = State::Inf;
          // cycle on neighbors
          for (int l = i - 1; l != i + 2; ++l) {
            for (int m = j - 1; m != j + 2; ++m) {
              int prob2 = rand() % 1000;
              if (current(l, m) == State::Susc && prob2 < beta * 1000) {
                // infection!
                next(l, m) = State::Inf;
              }
            }
          }
        }
      } else {
        if (next(i, j) != State::Inf) { // non può disinfettarsi in automatico!
          next(i, j) = current(i, j);
        }
      }
    }
  }
  return next;
}

Board draw(Board const &next) {
  const int cell_size = 15;
  const sf::Vector2f cell_vector = sf::Vector2f(cell_size, cell_size);
  const int width = next.size();
  const int height = width;
  const int n_cells = width * height;
  int delay = 100;
  bool redraw = true;
  const float fps = 1.0f;

  sf::RenderWindow window(
      sf::VideoMode(cell_size * width, cell_size * height + 50), "Covid");
  window.setFramerateLimit(fps);
  sf::Clock clock;

  while (window.isOpen()) {
    if (clock.getElapsedTime().asSeconds() >= fps) {
      redraw = true;
      clock.restart();
    };
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
      case (sf::Event::Closed): {
        window.close();
      }
      }
    }

    window.clear(sf::Color::White);

    for (int i = 0; i != width; ++i) {
      for (int j = 0; j != height; ++j) {
        sf::RectangleShape cell;
        cell.setPosition(i * cell_size, j * cell_size);
        cell.setSize(cell_vector);
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color::Black);
        if (next(i, j) == State::Susc) {
          cell.setFillColor(sf::Color::Blue);
        }
        if (next(i, j) == State::Inf) {
          cell.setFillColor(sf::Color::Red);
        }
        if (next(i, j) == State::Rec) {
          cell.setFillColor(sf::Color::Green);
        }
        window.draw(cell);
      }
    }
    window.display();
    // sf::sleep(sf::milliseconds(delay));
  }

  return 0;
}

int main() {
  int dim = 20;
  Board board(dim);
  board(1, 1) = State::Inf;
  board(1, 2) = State::Inf;
  board(4, 5) = State::Inf;
  board(11, 16) = State::Inf;
  board(5, 19) = State::Inf;
  draw(board);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  for (int i = 0; i != 10; ++i) {
    // std::cout << "\033c";
    board = evolve(board, 0.3, 0.4);
    draw(board);
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
