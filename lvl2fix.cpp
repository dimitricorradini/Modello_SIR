#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

enum class State
{
    Susc,
    Inf,
    Rec
};

class Board
{
private:
    int n_;
    std::vector<State> board_;

public:
    Board(int n) : n_(n), board_(n * n) {}

    State operator()(int i, int j) const
    {
        return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j] : State::Rec;
    }

    State &operator()(int i, int j)
    {
        assert(i >= 0 && i < n_ && j >= 0 && j < n_);
        return board_[i * n_ + j];
    }

    int size() const { return n_; }
};

Board evolve(Board const &current, double const beta, double const gamma)
{
    // add exception for value of beta, gamma
    if (beta > 1 || gamma > 1 || beta < 0 || gamma < 0)
    {
        throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"};
    }

    int n = current.size();
    Board next(n);
    //seed for random generation
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dist{0, 1};
    //cycle on matrix
    for (int i = 0; i != n; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            if (current(i, j) == State::Inf)
            {
                double prob1 = dist(gen);
                if (prob1 < gamma)
                {
                    //recovery/death
                    next(i, j) = State::Rec;
                }
                else
                {
                    next(i, j) = State::Inf;
                    //cycle on neighbors
                    for (int l = i - 1; l <= i + 1; ++l)
                    {
                        for (int m = j - 1; m <= j + 1; ++m)
                        {
                            double prob2 = dist(gen);
                            if (current(l, m) == State::Susc && prob2 < beta)
                            {
                                //infection!
                                next(l, m) = State::Inf;
                            }
                        }
                    }
                }
            }
            else
            {
                if (next(i, j) != State::Inf)
                { //non può disinfettarsi in automatico!
                    next(i, j) = current(i, j);
                }
            }
        }
    }
    return next;
}

struct Points
{
    float sus;
    float rec;
    float inf;
    float day;
};

static constexpr int heightG = 500;
static constexpr int widthG = 700;

int countI(Board const &board)
{
    int inf_ = 0;
    for (int l = 1; l <= board.size(); ++l)
    {
        for (int j = 1; j <= board.size(); ++j)
        {
            if (board(l, j) == State::Inf)
            {
                ++inf_;
            };
        };
    }
    return inf_;
}

std::vector<Points> set_points(Board const &board, Points point)
{
    std::vector<Points> point_;
    point.sus = heightG - 50;
    point.rec = heightG - 50;
    point.inf = heightG - 50;
    point.day = 50.f;

        if (pow(board.size(), 2) == heightG - 80)
        {
            for (int i = 0; i != board.size(); i++)
            {
                for (int j = 0; j != board.size(); j++)
                {
                    if (board(i, j) == State::Susc)
                    {
                        --point.sus;
                    }
                    if (board(i, j) == State::Rec)
                    {
                        --point.rec;
                    }
                    if (board(i, j) == State::Inf)
                    {
                        --point.inf;
                    }
                }
            }
        }
        if (pow(board.size(), 2) < heightG - 80)
        {
            for (int i = 0; i != board.size(); i++)
            {
                for (int j = 0; j != board.size(); j++)
                {
                    if (board(i, j) == State::Susc)
                    {
                        point.sus =point.sus - ((heightG - 80) / pow(board.size(), 2));
                    }
                    if (board(i, j) == State::Rec)
                    {
                        point.rec =point.rec - ((heightG - 80) / pow(board.size(), 2));
                    }
                    if (board(i, j) == State::Inf)
                    {
                        point.inf = point.inf - ((heightG - 80) / pow(board.size(), 2));
                    }
                }
            }
        }
        if (pow(board.size(), 2) > heightG - 80)
        {
            for (int i = 0; i != board.size(); i++)
            {
                for (int j = 0; j != board.size(); j++)
                {
                    if (board(i, j) == State::Susc)
                    {
                        point.sus =point.sus - (pow(board.size(), 2) / (heightG - 80));
                    }
                    if (board(i, j) == State::Rec)
                    {
                        point.rec =point.rec - (pow(board.size(), 2) / (heightG - 80));
                    }
                    if (board(i, j) == State::Inf)
                    {
                        point.inf =point.inf - (pow(board.size(), 2) / (heightG - 80));
                    }
                }
            }
        }
        ++point.day;
        point_.push_back(point);
    return point_;
}

//bisogna capire bene come funzionano vertexarray e il primitivetype::linestrip
class Graph
{
    sf::RenderWindow g_window;
    static constexpr auto g_window_title = "Trend";
    std::vector<Points> g_points;

public:
    Graph() : g_window{sf::VideoMode(widthG, heightG), g_window_title, sf::Style::Close}, g_points(0) {}

    void launch_graph()
    {
        sf::Event event;

        while (event.type != sf::Event::KeyPressed)
        {
            if (event.key.code != sf::Keyboard::Escape)
            {
                g_window.waitEvent(event);
            }
            if (event.key.code != sf::Keyboard::X)
            {
                g_window.close();
            }
            else
            {
                g_window.waitEvent(event);
            }
        }
    }
    int draw_axis()
    {
        const sf::Vector2f dimXaxis = sf::Vector2f(widthG, 3);
        const sf::Vector2f dimYaxis = sf::Vector2f(3, heightG);
        sf::RectangleShape Xaxis(dimXaxis);
        sf::RectangleShape Yaxis(dimYaxis);
        Xaxis.setPosition(0, heightG - 50);
        Xaxis.setFillColor(sf::Color::Black);
        Yaxis.setPosition(50, 0);
        Yaxis.setFillColor(sf::Color::Black);

        g_window.clear(sf::Color::White);

        g_window.draw(Xaxis);
        g_window.draw(Yaxis);

        g_window.display();
    }

    int draw(Board const &board, Points point)
    { //fix this!
        g_points = set_points(board, point);
        std::vector<Points>::iterator it;
        int a = -1;
        sf::VertexArray curveS(sf::PrimitiveType::LineStrip, g_points.size());
        sf::VertexArray curveR(sf::PrimitiveType::LineStrip, g_points.size());
        sf::VertexArray curveI(sf::PrimitiveType::LineStrip, g_points.size());

        for (it = g_points.begin(); it != g_points.end(); ++it)
        {
            ++a;
            point = g_points.at(a);
           
            sf::Vertex vertexS(sf::Vector2f(point.day, point.sus));
            sf::Vertex vertexR(sf::Vector2f(point.day, point.rec));
            sf::Vertex vertexI(sf::Vector2f(point.day, point.inf));

            curveS.append(vertexS);
            curveR.append(vertexR);
            curveI.append(vertexI);

            g_window.draw(curveS);
            g_window.draw(curveR);
            g_window.draw(curveI);
        }

        g_window.display();

        return 0;
    }
};

class Display
{

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
        : m_board_side{board_side}, m_window{
                                        sf::VideoMode(display_side(board_side), display_side(board_side)),
                                        s_window_title,
                                        sf::Style::Close}
    {
    }

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
                m_window.draw(cell);
            }
        }

        m_window.display();
        //window.clear(sf::Color::White);
        //std::cout << "\033c";

        return 0;
    }
};

int main()
{
    int dim = 30;
    Board board(dim);
    Points point;
    board(10, 26) = State::Inf;
    board(21, 27) = State::Inf;
    board(2, 2) = State::Inf;
    board(23, 26) = State::Inf;
    board(3, 9) = State::Inf;
    board(24, 25) = State::Inf;
    board(14, 2) = State::Inf;
    //creo la finetra grafica per la griglia, faccio partire la simulazione con enter
    Display display(dim);
    display.draw(board);
    display.wait_key_pressed();
    //la griglia fa il suo fino a che gli infetti sono 0, a quel punto si interrompe, nel mentre set_points raccoglie tutte le coord
    //dei punti da disegnare nel grafico dell'andamento, i punti delle coord sono tenuti dentro ad una struct
    //la struct viene pushata ogni volta dentro ad un vettore
    while (true)
    {
        board = evolve(board, 0.4, 0.4);
        display.draw(board);
        set_points(board, point);
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        if (countI(board) == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            break;
        }
    }
    //creo la finetsra, disegno gli assi, lancio la rappresentazione dell'andamento con enter, mi disegna l'andamento
    Graph graph;
    graph.draw_axis();
    graph.launch_graph();
    graph.draw(board, point);
}
