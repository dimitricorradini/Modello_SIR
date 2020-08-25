#ifndef SIR_DISPLAY_HPP
#define SIR_DISPLAY_HPP

#include <SFML/Graphics.hpp>
#include "board.hpp"

namespace SIR 
{
  class Display
  {
    private:
    int m_board_side;
    sf::RenderWindow m_window;

    static constexpr int cell_size = 10;

    static int display_side(int board_side)
    {
      return board_side * cell_size;
    }

    static constexpr auto s_window_title = "Virus simulation";

    public:
    Display(int board_side)
      : m_board_side{board_side}, m_window {
                                  sf::VideoMode(display_side(board_side), display_side(board_side)),
                                  s_window_title,
                                  sf::Style::Close } {}

    void wait_key_pressed()
    {
      sf::Event event;
      m_window.waitEvent(event);
      while (event.type != sf::Event::KeyPressed)
      {
        if (event.key.code != sf::Keyboard::Escape)
        {
          m_window.waitEvent(event);
        }
      }
    }

    void closing()
    {
      sf::Event e;
      m_window.waitEvent(e);
      while(e.key.code != sf::Keyboard::Q)
      {
        m_window.waitEvent(e);
        if(e.key.code == sf::Keyboard::Q)
        {
          m_window.close();
        }
      }
    }

    void show_instructions(Board const& board)
    {
      const sf::Vector2f dim_box((m_board_side * cell_size)*0.8,(m_board_side * cell_size)*0.6);
      sf::RectangleShape box(dim_box);
      box.setPosition(80,80);
      m_window.clear(sf::Color::White);
        
      /*sf::Texture t_box;
      t_box.loadFromFile("/tmp/mozilla_tasso0/info_text-2.jpg");
      if((t_box.loadFromFile("/tmp/mozilla_tasso0/info_text-2.jpg")))
      {
        std::cout << '8' << '\n';
      }*/
        
      box.setFillColor(sf::Color::White);
      box.setTexture(&t_box);
      m_window.draw(box);
      m_window.display();
    }

    void click(Board &board)
    {
      sf::Vector2i mouse;
      sf::Event e;
      m_window.waitEvent(e);
      bool selection = true;
      while(selection)
      {
        m_window.waitEvent(e);
        while(e.key.code != sf::Mouse::Left)
        {
          m_window.waitEvent(e);
          if(e.key.code == sf::Mouse::Left)
          {
            mouse = sf::Mouse::getPosition(m_window);
            if(mouse.x < 0 || mouse.y < 0 || mouse.x > (m_board_side*cell_size) || mouse.y > (m_board_side*cell_size))
            {
              for (int i = 0; i != m_board_side; ++i)
              {
                if((i*10) < mouse.x && ((i+1)*10) > mouse.x)
                {
                  for (int j = 0; j != m_board_side; ++j)
                  {
                    if((j*10) < mouse.y && ((j+1)*10) > mouse.y)
                    {
                      board(i,j) = State::Inf;
                      draw(board);
                    }
                  }
                }
              }
            }
            if (e.key.code == sf::Keyboard::S)
            {
              selection = false;
            }
          }
        }  
      }
    }

    int draw(Board const &board)
    {
      const sf::Vector2f cell_vector = sf::Vector2f(cell_size, cell_size);

      sf::RectangleShape cell(cell_vector);
      cell.setOutlineThickness(1);
      cell.setOutlineColor(sf::Color::Black);

      m_window.clear(sf::Color::White);

      for (int i = 0; i != m_board_side; ++i)
      {
        for (int j = 0; j != m_board_side; ++j)
        {
          cell.setPosition(i * cell_size, j * cell_size);

          if (board(i, j) == State::Susc)
          {
            cell.setFillColor(sf::Color::Blue);
          }
          if (board(i, j) == State::Inf)
          {
            cell.setFillColor(sf::Color::Red);
          }
          if (board(i, j) == State::Rec)
          {
            cell.setFillColor(sf::Color::Green);
          }
          if (board(i, j) == State::Dead)
          {
            cell.setFillColor(sf::Color::Black);
          }
          m_window.draw(cell);
        }
      }
      /*for (int i = 0; i != m_board_side; ++i)
      {
        for (int j = 0; j != m_board_side; ++j)
        {
          if(board(i,j)==State::Dead)
          {
            sf::RectangleShape cell1(cell_vector);
            cell1.setPosition(i * cell_size, j * cell_size);
            cell1.setOutlineThickness(1);
            cell1.setOutlineColor(sf::Color::Black);
            sf::Texture t_dead;
            t_dead.loadFromFile("/home/tasso/Downloads/dead.png");
            cell.setFillColor(sf::Color::White);
            cell1.setTexture(&t_dead);
            m_window.draw(cell1);
          }
        }
      }*/

      m_window.display();
      return 0;
    }
  };
//points should be here?
}

#endif