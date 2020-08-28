#ifndef SIR_DISPLAY_HPP
#define SIR_DISPLAY_HPP

#include <SFML/Graphics.hpp>

namespace SIR
{
  class Board;

  class Display
  {
    private:
    int const m_board_side;
	  sf::RenderWindow m_window;

	  static constexpr int cell_side = 10;

	  static int display_side(int board_side)
	  {
      return board_side * cell_side;
	  }

	  static constexpr auto s_window_title = "Virus simulation";

	  public:
	  Display(int board_side)
	    : m_board_side{board_side}, m_window { 
											  sf::VideoMode(display_side(board_side), display_side(board_side)),
											  s_window_title,
											  sf::Style::Titlebar} {}

	  void wait_key_pressed();

	  void closing();

	  void show_instructions();

	  void click(Board &board);

    int draw(Board const &board);
  };
}

#endif
