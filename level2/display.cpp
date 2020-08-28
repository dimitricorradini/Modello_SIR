#include <iostream>
#include "display.hpp"
#include "board.hpp"

void SIR::Display::wait_key_pressed()
{
	sf::Event wait_key;
	m_window.waitEvent(wait_key);
	while (wait_key.type != sf::Event::KeyPressed)
	{
	  if (wait_key.key.code != sf::Keyboard::Escape)
		{
		 	m_window.waitEvent(wait_key);
		}
	}
}

void SIR::Display::closing()
{
	sf::Event close1;
	m_window.waitEvent(close1);
	while (close1.key.code != sf::Keyboard::Q)
	{
	  m_window.waitEvent(close1);
	  if (close1.key.code == sf::Keyboard::Q)
		{
		  m_window.close();
		}
	}
}

void SIR::Display::show_instructions()
{
	  	sf::Event skip1;
	  	m_window.waitEvent(skip1);

	  	sf::Vector2f dim_box(300, 150);
	  	sf::Vector2f dim_box1(300, 300);

	  	sf::RectangleShape box(dim_box);
	  	sf::RectangleShape box1(dim_box1);

	  	box1.setFillColor(sf::Color::White);
	  	box1.setPosition(50, 50);
	  	box.setFillColor(sf::Color::White);
	  	box.setPosition(50, 50);

	  	sf::Font font;
	  	font.loadFromFile("../utilities/arial.ttf");
	  	sf::Text quest("   Welcome to our pandemic simulator! ", font);
	  	quest.setFillColor(sf::Color::Black);
	  	quest.setCharacterSize(15);
	  	quest.setPosition(65, 75);
	  	sf::Text quest_1("Press S to continue.", font);
	  	quest_1.setFillColor(sf::Color::Black);
	  	quest_1.setCharacterSize(10);
	  	quest_1.setPosition(230, 180);
	  	sf::Text quest_2("Press Q to continue.", font);
	  	quest_2.setPosition(230, 330);
	  	quest_2.setFillColor(sf::Color::Black);
	  	quest_2.setCharacterSize(10);
	  	m_window.draw(box);
	  	m_window.draw(quest);
	  	m_window.draw(quest_1);
	  	m_window.display();

	  	while (skip1.key.code != sf::Keyboard::S)
	  	{
				m_window.waitEvent(skip1);
				if (skip1.key.code == sf::Keyboard::S)
				{
		 	 		m_window.clear(sf::Color::Black);
		 	  	m_window.clear(sf::Color::Black);
		  		quest.setFillColor(sf::Color::Black);
		  		quest.setString("1) Choose!\nStart your pandemic clicking\non the cells you want to infect.\n2) Simulation!\nPress S to start the simulation,\nthen press Q to close simulation window.\n3) Trend!\nPress S to display the trend\nof the pandemic,\nthen close the window to quit.");
		  		quest.setCharacterSize(15);
		  		quest.setPosition(65, 75);
		  		m_window.draw(box1);
		  		m_window.draw(quest);
		  		m_window.draw(quest_2);
		  		m_window.display();
				}
	  	}

	  	sf::Event skip2;
	  	m_window.waitEvent(skip2);

	  	while (skip2.key.code != sf::Keyboard::Q)
	  	{
				m_window.waitEvent(skip2);
				if (skip2.key.code == sf::Keyboard::Q)
				{
		  		m_window.close();
				}
	  	}
  	}

void SIR::Display::click(SIR::Board &board)
		{
    	sf::Vector2i mouse;
	  	sf::Event click;
	  	m_window.waitEvent(click);
	  	//while (e.type != sf::Event::KeyPressed) {
	  	//(click.type != sf::Event::KeyPressed)
	  	while (click.key.code != sf::Keyboard::S)
	  	{
				m_window.waitEvent(click);
				if (click.type == sf::Event::MouseButtonPressed)
				{
		  		if (click.mouseButton.button == sf::Mouse::Button::Left)
		  		{
						mouse = sf::Mouse::getPosition(m_window);
						for (int i = 0; i != m_board_side; ++i)
						{
			  			if ((i * 10) < mouse.x && ((i + 1) * 10) > mouse.x)
			  			{
								for (int j = 0; j != m_board_side; ++j)
								{
				  				if ((j * 10) < mouse.y && ((j + 1) * 10) > mouse.y)
				  				{
										board(i, j) = State::Inf;
										draw(board);
				  				}
								}
			  			}
						}
					}
	      	if (click.mouseButton.button == sf::Mouse::Button::Right)
		  		{
						mouse = sf::Mouse::getPosition(m_window);
						for (int i = 0; i != m_board_side; ++i)
						{
			  			if ((i * 10) < mouse.x && ((i + 1) * 10) > mouse.x)
			  			{
								for (int j = 0; j != m_board_side; ++j)
								{
				  				if ((j * 10) < mouse.y && ((j + 1) * 10) > mouse.y)
				  				{
										board(i, j) = State::Susc;
										draw(board);
				  				}
								}
			  			}
						}
		  		}
				}
				if (click.key.code == sf::Keyboard::S)
				{
					std::cout << "premuto tasti" << '\n';
				}
	  	}
	  //}
		}

int SIR::Display::draw(SIR::Board const &board)
		{
	  	const sf::Vector2f cell_vector = sf::Vector2f(cell_side, cell_side);

	  	sf::RectangleShape cell(cell_vector);
	  	sf::RectangleShape cell1(cell_vector);
	  	cell.setOutlineThickness(1);
	  	cell.setOutlineColor(sf::Color::Black);
      
    	cell1.setOutlineThickness(1);
	  	cell1.setOutlineColor(sf::Color::Black);
	  	sf::Texture t_dead;
	  	t_dead.loadFromFile("../utilities/dead.png");
	  	m_window.clear(sf::Color::White);

	  	for (int i = 0; i != m_board_side; ++i)
	  	{
	    	for (int j = 0; j != m_board_side; ++j)
	    	{
		  		cell.setPosition(i * cell_side, j * cell_side);

		  		if (board(i, j) == State::Susc)
		  		{
						cell.setFillColor(sf::Color::Blue);
						m_window.draw(cell);
		  		}
		  		if (board(i, j) == State::Inf)
		  		{
						cell.setFillColor(sf::Color::Red);
						m_window.draw(cell);
		  		}
		  		if (board(i, j) == State::Rec)
		  		{
						cell.setFillColor(sf::Color::Green);
						m_window.draw(cell);
		  		}
		  		if (board(i, j) == State::Dead)
		  		{
						cell1.setPosition(i * cell_side, j * cell_side);
						cell1.setFillColor(sf::Color::White);
						cell1.setTexture(&t_dead);
						m_window.draw(cell1);
		  		}
				}
	  	}
	  	m_window.display();
	  	return 0;
  	}