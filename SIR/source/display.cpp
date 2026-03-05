#include "display.hpp"
#include "board.hpp"

void SIR::Display::closing() {
  while (true) {
    std::optional<sf::Event> close2 = m_window.waitEvent();
    if (!close2) continue;
    if (const auto* key = close2->getIf<sf::Event::KeyPressed>()) {
      if (key->code == sf::Keyboard::Key::Q) {
        m_window.close();
        break;
      }
    }
  }
}

void SIR::Display::click(SIR::Board &board) {
  while (true) {
    std::optional<sf::Event> click = m_window.waitEvent();
    if (!click) continue;

    if (const auto* key = click->getIf<sf::Event::KeyPressed>()) {
      if (key->code == sf::Keyboard::Key::S) break;
    }

    if (const auto* mouse_event = click->getIf<sf::Event::MouseButtonPressed>()) {
      if (mouse_event->button == sf::Mouse::Button::Left) {
        sf::Vector2i mouse = sf::Mouse::getPosition(m_window);
        for (int i = 0; i != m_board_side; ++i) {
          if ((i * cell_side < mouse.x) && ((i + 1) * cell_side > mouse.x)) {
            for (int j = 0; j != m_board_side; ++j) {
              if ((j * cell_side < mouse.y) && ((j + 1) * cell_side > mouse.y)) {
                if (board(i, j) == State::Susc) {
                  board(i, j) = State::Inf;
                  draw(board);
                } else {
                  board(i, j) = State::Susc;
                  draw(board);
                }
              }
            }
          }
        }
      }
    }
  }
}

int SIR::Display::draw(SIR::Board const &board) {
  const sf::Vector2f cell_vector{cell_side, cell_side};
  sf::RectangleShape cell(cell_vector);
  sf::RectangleShape cell1(cell_vector);
  cell.setOutlineThickness(1);
  cell.setOutlineColor(sf::Color::Black);
  cell1.setOutlineThickness(1);
  cell1.setOutlineColor(sf::Color::Black);
  sf::Texture t_dead;
  if (!t_dead.loadFromFile("../utilities/dead.png")) {
    // texture failed to load, cell1 will render without it
  }
  m_window.clear(sf::Color::White);
  for (int i = 0; i != m_board_side; ++i) {
    for (int j = 0; j != m_board_side; ++j) {
      cell.setPosition(sf::Vector2f(i * cell_side, j * cell_side));
      if (board(i, j) == State::Susc) {
        cell.setFillColor(sf::Color::Blue);
        m_window.draw(cell);
      }
      if (board(i, j) == State::Inf) {
        cell.setFillColor(sf::Color::Red);
        m_window.draw(cell);
      }
      if (board(i, j) == State::Rec) {
        cell.setFillColor(sf::Color::Green);
        m_window.draw(cell);
      }
      if (board(i, j) == State::Dead) {
        cell1.setPosition(sf::Vector2f(i * cell_side, j * cell_side));
        cell1.setFillColor(sf::Color::White);
        cell1.setTexture(&t_dead);
        m_window.draw(cell1);
      }
    }
  }
  m_window.display();
  return 0;
}