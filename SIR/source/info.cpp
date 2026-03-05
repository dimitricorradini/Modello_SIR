#include "info.hpp"

void SIR::Info::closing() {
  while (true) {
    std::optional<sf::Event> close1 = info_window.waitEvent();
    if (!close1) continue;
    if (const auto* key = close1->getIf<sf::Event::KeyPressed>()) {
      if (key->code == sf::Keyboard::Key::Q) {
        info_window.close();
        break;
      }
    }
  }
}

void SIR::Info::show_instructions() {
  sf::Vector2f dim_box(400, 200);
  sf::Vector2f dim_box1(500, 600);
  sf::RectangleShape box(dim_box);
  sf::RectangleShape box1(dim_box1);

  box.setFillColor(sf::Color::White);
  float pos_x = (info_window_side - dim_box.x) / 2;
  float pos_y = (info_window_side - dim_box.y) / 2;
  box.setPosition(sf::Vector2f(pos_x, pos_y));

  box1.setFillColor(sf::Color::White);
  float pos1_x = (info_window_side - dim_box1.x) / 2;
  float pos1_y = (info_window_side - dim_box1.y) / 2;
  box1.setPosition(sf::Vector2f(pos1_x, pos1_y));

  sf::Font font;
  font.openFromFile("../utilities/arial.ttf");

  sf::Text quest(font, "Welcome to our pandemic simulator!");
  quest.setFillColor(sf::Color::Black);
  quest.setCharacterSize(20);
  quest.setPosition(sf::Vector2f(pos_x + 30, pos_y + 20));

  sf::Text quest_1(font, "Press S to continue.");
  quest_1.setFillColor(sf::Color::Black);
  quest_1.setCharacterSize(15);
  quest_1.setPosition(sf::Vector2f(pos_x + 250, pos_y + 170));

  sf::Text quest_2(font, "Press Q to continue.");
  quest_2.setPosition(sf::Vector2f(pos1_x + 350, pos1_y + 570));
  quest_2.setFillColor(sf::Color::Black);
  quest_2.setCharacterSize(15);

  info_window.draw(box);
  info_window.draw(quest);
  info_window.draw(quest_1);
  info_window.display();

  // Wait for S key to show instructions page
  while (true) {
    std::optional<sf::Event> skip = info_window.waitEvent();
    if (!skip) continue;
    if (const auto* key = skip->getIf<sf::Event::KeyPressed>()) {
      if (key->code == sf::Keyboard::Key::S) {
        info_window.clear(sf::Color::Black);
        quest.setFillColor(sf::Color::Black);
        quest.setString(
            "1) Choose!\n\nStart your pandemic by clicking\non the cells you "
            "want to infect\nwith the left mouse button.\nClick again to undo "
            "your choice.\n\n\n2) Simulation!\n\nPress S to start the "
            "simulation,\nthen press Q to close simulation window\nwhen the "
            "evolution halts.\n\n\n3) Trend!\n\nAfter closing the simulation "
            "window with Q,\nthe graph of the trend will be displayed.\nClose "
            "the Trend window to quit.");
        quest.setCharacterSize(20);
        quest.setPosition(sf::Vector2f(pos1_x + 20, pos1_y + 20));
        info_window.draw(box1);
        info_window.draw(quest);
        info_window.draw(quest_2);
        info_window.display();
        break;
      }
    }
  }

  closing();
}