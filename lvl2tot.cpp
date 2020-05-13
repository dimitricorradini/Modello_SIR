#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

constexpr int heightG = 500;
constexpr int widthG = 700;
int day=50;

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

int count(State const& state,Board const& board) {
  int c=heightG-50;
  State x;
  if(pow(board.size(),2)==heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          --c;
        }
      }
    }
  }
  if(pow(board.size(),2)<heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          c=-floor((heightG-80)/pow(board.size(),2));
        }
      }
    }
  }
    if(pow(board.size(),2)>heightG-80) {
    for (int i = 0; i != board.size(); i++) {
      for (int j = 0; j != board.size(); j++) {
        x = board(i, j);
        if (x== state) {
          c=-(pow(board.size(),2)/(heightG-80));
        }
      }
    }
    c=floor(c);
  }
  return c;
}

int countR(Board const &board) {
  int r = 0;
  for (int l = 1; l <= board.size(); ++l) {
    for (int j = 1; j <= board.size(); ++j) {
      if (board(l, j) == State::Rec) {
        ++r;
      };
    };
  }
  return r;
}

int Graph(Board const &board) {
  const sf::Vector2f dimXaxis = sf::Vector2f(widthG, 3);
  const sf::Vector2f dimYaxis = sf::Vector2f(3, heightG);

  sf::RenderWindow graph(sf::VideoMode(widthG, heightG), "Covid-19");

  while (graph.isOpen()) {

    sf::Event event;

    while (graph.pollEvent(event)) {

      if (event.type == sf::Event::Closed) {
        graph.close();
      };
    }

    graph.clear(sf::Color::White);

    sf::RectangleShape Xaxis;
    Xaxis.setPosition(0, heightG - 50);
    Xaxis.setSize(dimXaxis);
    Xaxis.setFillColor(sf::Color::Black);
    sf::RectangleShape Yaxis;
    Yaxis.setPosition(50, 0);
    Yaxis.setSize(dimYaxis);
    Yaxis.setFillColor(sf::Color::Black);

    graph.draw(Xaxis);
    graph.draw(Yaxis);
    
    sf::Vector2f coordS = sf::Vector2f(day, count(State::Susc,board));
    sf::Vector2f coordR = sf::Vector2f(day, count(State::Rec,board));
    sf::Vector2f coordI = sf::Vector2f(day, count(State::Inf,board));
      
    sf::VertexArray curveS(sf::PrimitiveType::LineStrip, 0);
    sf::VertexArray curveR(sf::PrimitiveType::LineStrip, 0);
    sf::VertexArray curveI(sf::PrimitiveType::LineStrip, 0);
    /*
    while(countR(board)!=pow(board.size(),2)) {
      day+=15;
      curveS.append(sf::Vertex(sf::Vector2f(day, count(State::Susc,board))));
      curveR.append(sf::Vertex(sf::Vector2f(day, count(State::Rec,board))));
      curveI.append(sf::Vertex(sf::Vector2f(day, count(State::Inf,board))));
*/
      curveS.append(sf::Vertex(sf::Vector2f(day, count(State::Susc,board))));
      curveR.append(sf::Vertex(sf::Vector2f(day, count(State::Rec,board))));
      curveI.append(sf::Vertex(sf::Vector2f(day, count(State::Inf,board))));
   // }

    graph.draw(curveS);
    graph.draw(curveR);
    graph.draw(curveI);

    graph.display();
    graph.clear(sf::Color::White);
  }
  return 0;
}

int draw(Board const &board) {
  const int cell_size = 6;
  const sf::Vector2f cell_vector = sf::Vector2f(cell_size, cell_size);
  const int width = board.size();
  const int height = width;
  const int n_cells = width * height;
  int delay = 100;
  bool redraw = true;
  const float fps = 1.0f;

  sf::RenderWindow window(sf::VideoMode(cell_size * width, cell_size * height),
                          "Covid");
  /*window.setFramerateLimit(fps);
  sf::Clock clock;*/

  while (window.isOpen()) {
    /* if (clock.getElapsedTime().asSeconds() >= fps) {
       redraw = true;
       clock.restart();
         };*/

    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      };
    }

    window.clear(sf::Color::White);

    for (int i = 0; i != width; ++i) {
      for (int j = 0; j != height; ++j) {
        sf::RectangleShape cell;
        cell.setPosition(i * cell_size, j * cell_size);
        cell.setSize(cell_vector);
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color::Black);
        if (board(i, j) == State::Susc) {
          cell.setFillColor(sf::Color::Blue);
        }
        if (board(i, j) == State::Inf) {
          cell.setFillColor(sf::Color::Red);
        }
        if (board(i, j) == State::Rec) {
          cell.setFillColor(sf::Color::Green);
        }
        window.draw(cell);
      }
    }

    window.display();
    window.clear(sf::Color::White);
    sf::sleep(sf::milliseconds(delay));
    std::cout << "\033c";
  }
  return 0;
}

int main() {
  int dim = 50;
  Board board(dim);
  board(1, 1) = State::Inf;
  board(1, 2) = State::Inf;
  board(4, 5) = State::Inf;
  board(11, 16) = State::Inf;
  board(5, 19) = State::Inf;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  for (int i = 0; i != 3; ++i) {
    // std::cout << "\033c";
    if (i == 0) {
      draw(board);
      Graph(board);
    } else {
      board = evolve(board, 0.3, 0.4);
      draw(board);
      Graph(board);
      // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}
