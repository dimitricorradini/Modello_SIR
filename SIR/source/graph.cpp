#include "graph.hpp"
#include "board.hpp"
#include <cmath>
#include <string>

void SIR::Graph::closing() {
  while (true) {
    std::optional<sf::Event> close3 = g_window.waitEvent();
    if (!close3) continue;
    if (close3->is<sf::Event::Closed>()) {
      g_window.close();
      break;
    }
  }
}

int SIR::Graph::draw(SIR::Board const &board, std::vector<Points> &g_points) {
  int r = -1;
  double const d = board.side() * board.side();
  double const x = (heightG - 100) / d;

  const sf::Vector2f dimXaxis(widthG - 55, 2);
  const sf::Vector2f dimYaxis(2, heightG - 100);
  const sf::Vector2f arrow(10, 2);
  const sf::Vector2f arrow1(2, 10);
  const sf::Vector2f legend_box(310, 45);
  const sf::Vector2f lines(30, 2);
  const sf::Vector2f p_line(8, 1);
  const sf::Vector2f p_line1(12, 2);

  sf::RectangleShape pop_line(p_line);
  sf::RectangleShape pop_line1(p_line1);
  sf::RectangleShape lineS(lines);
  sf::RectangleShape lineR(lines);
  sf::RectangleShape lineI(lines);
  sf::RectangleShape lineD(lines);
  sf::RectangleShape legend(legend_box);
  sf::RectangleShape Xaxis(dimXaxis);
  sf::RectangleShape Yaxis(dimYaxis);
  sf::RectangleShape x_arrow(arrow);
  sf::RectangleShape y_arrow(arrow1);
  sf::RectangleShape x_arrow1(arrow);
  sf::RectangleShape y_arrow1(arrow1);

  pop_line.setFillColor(sf::Color::Black);
  pop_line1.setFillColor(sf::Color::Black);

  lineS.setPosition(sf::Vector2f(widthG - 226, 38));
  lineR.setPosition(sf::Vector2f(widthG - 145, 38));
  lineI.setPosition(sf::Vector2f(widthG - 72, 38));
  lineD.setPosition(sf::Vector2f(widthG - 298, 38));
  lineS.setFillColor(sf::Color::Blue);
  lineR.setFillColor(sf::Color::Green);
  lineI.setFillColor(sf::Color::Red);
  lineD.setFillColor(sf::Color::Black);

  legend.setPosition(sf::Vector2f(widthG - legend_box.x - 2, 2));
  legend.setFillColor(sf::Color::White);
  legend.setOutlineThickness(2);
  legend.setOutlineColor(sf::Color::Black);

  y_arrow.setPosition(sf::Vector2f(50, 50));
  y_arrow.setFillColor(sf::Color::Black);
  y_arrow.setRotation(sf::degrees(30));

  y_arrow1.setPosition(sf::Vector2f(50, 50));
  y_arrow1.setRotation(sf::degrees(-30));
  y_arrow1.setFillColor(sf::Color::Black);

  x_arrow.setPosition(sf::Vector2f(widthG - 11, heightG - 55));
  x_arrow.setFillColor(sf::Color::Black);
  x_arrow.setRotation(sf::degrees(30));

  x_arrow1.setFillColor(sf::Color::Black);
  x_arrow1.setPosition(sf::Vector2f(widthG - 11, heightG - 45));
  x_arrow1.setRotation(sf::degrees(-30));

  Xaxis.setPosition(sf::Vector2f(50, heightG - 50));
  Xaxis.setFillColor(sf::Color::Black);
  Yaxis.setPosition(sf::Vector2f(50, 50));
  Yaxis.setFillColor(sf::Color::Black);

  g_window.clear(sf::Color::White);

  sf::Font font;
  font.openFromFile("../utilities/arial.ttf");

  sf::Text Xaxis_name(font, "Days");
  sf::Text Yaxis_name(font, "Population");

  std::string n_day;
  std::string pop_s;

  sf::Text pop(font, pop_s);
  sf::Text sus(font, "Susceptible");
  sf::Text rec(font, "Recovered");
  sf::Text inf(font, "Infected");
  sf::Text dead(font, "Deceased");
  sf::Text text(font, n_day);
  sf::Text leg(font, "Legend:");

  Xaxis_name.setStyle(sf::Text::Bold);
  Xaxis_name.setPosition(sf::Vector2f(widthG - 45, heightG - 25));
  Xaxis_name.setCharacterSize(15);
  Xaxis_name.setFillColor(sf::Color::Black);

  Yaxis_name.setStyle(sf::Text::Bold);
  Yaxis_name.setPosition(sf::Vector2f(10, 30));
  Yaxis_name.setCharacterSize(15);
  Yaxis_name.setFillColor(sf::Color::Black);

  leg.setStyle(sf::Text::Bold);
  leg.setPosition(sf::Vector2f(widthG - 306, 3));
  leg.setCharacterSize(15);
  leg.setFillColor(sf::Color::Black);

  dead.setStyle(sf::Text::Regular);
  dead.setCharacterSize(13);
  dead.setFillColor(sf::Color::Black);
  dead.setPosition(sf::Vector2f(widthG - 298, 20));

  sus.setStyle(sf::Text::Regular);
  sus.setCharacterSize(13);
  sus.setFillColor(sf::Color::Blue);
  sus.setPosition(sf::Vector2f(widthG - 226, 20));

  rec.setStyle(sf::Text::Regular);
  rec.setCharacterSize(13);
  rec.setFillColor(sf::Color::Green);
  rec.setPosition(sf::Vector2f(widthG - 145, 20));

  inf.setStyle(sf::Text::Regular);
  inf.setCharacterSize(13);
  inf.setFillColor(sf::Color::Red);
  inf.setPosition(sf::Vector2f(widthG - 72, 20));

  pop.setStyle(sf::Text::Bold);
  pop.setCharacterSize(10);
  pop.setFillColor(sf::Color::Black);

  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(10);
  text.setFillColor(sf::Color::Black);

  std::vector<int> v_text;
  std::vector<int> v_pop;

  sf::VertexArray curveS(sf::PrimitiveType::TriangleStrip, 2 * g_points.size());
  sf::VertexArray curveR(sf::PrimitiveType::TriangleStrip, 2 * g_points.size());
  sf::VertexArray curveI(sf::PrimitiveType::TriangleStrip, 2 * g_points.size());
  sf::VertexArray curveD(sf::PrimitiveType::TriangleStrip, 2 * g_points.size());

  sf::CircleShape PointS(3, 50);
  sf::CircleShape PointR(3, 50);
  sf::CircleShape PointI(3, 50);
  sf::CircleShape PointD(3, 50);

  PointS.setFillColor(sf::Color::Blue);
  PointR.setFillColor(sf::Color::Green);
  PointI.setFillColor(sf::Color::Red);
  PointD.setFillColor(sf::Color::Black);

  for (unsigned int a = 0; a != g_points.size(); a++) {
    float day = 50 + round(a * (widthG - 50) / g_points.size());
    auto point = g_points[a];

    curveS[2 * a].position = sf::Vector2f(day, point.sus);
    curveS[2 * a + 1].position = sf::Vector2f(day, point.sus + 3);
    PointS.setPosition(sf::Vector2f(day, point.sus - 2));
    curveS[2 * a].color = sf::Color::Blue;
    curveS[2 * a + 1].color = sf::Color::Blue;

    curveR[2 * a].position = sf::Vector2f(day, point.rec);
    curveR[2 * a + 1].position = sf::Vector2f(day, point.rec + 3);
    PointR.setPosition(sf::Vector2f(day, point.rec - 2));
    curveR[2 * a].color = sf::Color::Green;
    curveR[2 * a + 1].color = sf::Color::Green;

    curveI[2 * a].position = sf::Vector2f(day, point.inf);
    curveI[2 * a + 1].position = sf::Vector2f(day, point.inf + 3);
    PointI.setPosition(sf::Vector2f(day, point.inf - 2));
    curveI[2 * a].color = sf::Color::Red;
    curveI[2 * a + 1].color = sf::Color::Red;

    curveD[2 * a].position = sf::Vector2f(day, point.dead);
    curveD[2 * a + 1].position = sf::Vector2f(day, point.dead + 3);
    PointD.setPosition(sf::Vector2f(day, point.dead - 2));
    curveD[2 * a].color = sf::Color::Black;
    curveD[2 * a + 1].color = sf::Color::Black;

    if (a % 5 == 0) {
      v_text.push_back(a);
    }

    g_window.draw(PointS);
    g_window.draw(PointR);
    g_window.draw(PointI);
    g_window.draw(PointD);
  }

  for (unsigned int b = 0; b != v_text.size(); b++) {
    n_day = std::to_string(v_text[b]);
    float day1 = 50 + 5 * round(b * (widthG - 50) / g_points.size());
    text.setPosition(sf::Vector2f(day1 + 1, heightG - 42));
    text.setString(n_day);
    g_window.draw(text);
  }

  for (int y = 0; y <= heightG - 100; y += (250 * x)) {
    r++;
    sf::Vector2f pos_pop(46, heightG - 50 - y);
    sf::Vector2f pos_pop1(44, heightG - 50 - y);
    sf::Vector2f p_pop(20, (heightG - 50) - y);
    pop_s = std::to_string(r * 250);

    if (r % 4 == 0) {
      pop.setString(pop_s);
      pop.setPosition(p_pop);
      pop_line1.setPosition(pos_pop1);
      g_window.draw(pop_line1);
      g_window.draw(pop);
    } else {
      pop.setString(pop_s);
      pop.setPosition(p_pop);
      pop_line.setPosition(pos_pop);
      g_window.draw(pop_line);
      g_window.draw(pop);
    }
  }

  g_window.draw(curveR);
  g_window.draw(curveI);
  g_window.draw(curveS);
  g_window.draw(curveD);

  g_window.draw(legend);
  g_window.draw(leg);
  g_window.draw(sus);
  g_window.draw(rec);
  g_window.draw(inf);
  g_window.draw(dead);
  g_window.draw(lineS);
  g_window.draw(lineR);
  g_window.draw(lineI);
  g_window.draw(lineD);

  g_window.draw(Xaxis);
  g_window.draw(Yaxis);
  g_window.draw(Xaxis_name);
  g_window.draw(Yaxis_name);
  g_window.draw(x_arrow);
  g_window.draw(x_arrow1);
  g_window.draw(y_arrow);
  g_window.draw(y_arrow1);

  g_window.display();
  return 0;
}