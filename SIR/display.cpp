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
	sf::Event close2;
	m_window.waitEvent(close2);

	while (close2.key.code != sf::Keyboard::Q)
	{
		m_window.waitEvent(close2);
		if (close2.key.code == sf::Keyboard::Q)
		{
			m_window.close();
		}
	}
}

void SIR::Display::click(SIR::Board& board)
{
	sf::Vector2i mouse;
	sf::Event click;
	m_window.waitEvent(click);
	while ((click.type == sf::Event::MouseMoved) || (click.key.code != sf::Keyboard::S))
	{
		m_window.waitEvent(click);
		if (click.type == sf::Event::MouseButtonPressed)
		{
			if (click.mouseButton.button == sf::Mouse::Button::Left)
			{
				mouse = sf::Mouse::getPosition(m_window);
				for (int i = 0; i != m_board_side; ++i)
				{
					if ((i * cell_side < mouse.x) && ((i + 1) * cell_side > mouse.x))
					{
						for (int j = 0; j != m_board_side; ++j)
						{
							if ((j * cell_side < mouse.y) && ((j + 1) * cell_side > mouse.y))
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
					if ((i * cell_side < mouse.x) && ((i + 1) * cell_side > mouse.x))
					{
						for (int j = 0; j != m_board_side; ++j)
						{
							if ((j * cell_side < mouse.y) && ((j + 1) * cell_side > mouse.y))
							{
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

int SIR::Display::draw(SIR::Board const& board)
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