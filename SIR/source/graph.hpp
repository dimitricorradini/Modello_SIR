#ifndef SIR_GRAPH_HPP
#define SIR_GRAPH_HPP

#include <SFML/Graphics.hpp>
#include <vector>

namespace SIR {
class Board;

class Points;

class Graph {
private:
  sf::RenderWindow g_window;
  static constexpr auto g_window_title = "Trend";

public:
  Graph()
      : g_window{sf::VideoMode(widthG, heightG), g_window_title,
                 sf::Style::Close} {}

  void closing();

  int draw(Board const &board, std::vector<Points> &g_points);
};
} // namespace SIR

#endif
