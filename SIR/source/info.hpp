#ifndef SIR_INFO_HPP
#define SIR_INFO_HPP

#include <SFML/Graphics.hpp>

namespace SIR {
class Info {
private:
  sf::RenderWindow info_window;

  static constexpr int info_window_side = 700;

  static constexpr auto info_window_title = "Instructions";

public:
  Info()
      : info_window{sf::VideoMode(info_window_side, info_window_side),
                    info_window_title, sf::Style::Titlebar} {}

  void closing();

  void show_instructions();
};
} // namespace SIR

#endif
