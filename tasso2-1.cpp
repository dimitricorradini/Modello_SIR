#include <SFML/Graphics.hpp>
#include <cassert>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

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
    //float day;
};

static constexpr int heightG = 700;
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


Points set_points(Board const &board, Points &p)
{
    Points p1;
    p1.inf=0.f;
    p1.rec=0.f;
    p1.sus=0.f;
    p.sus=0.f;
    p.rec=0.f;
    p.inf=0.f;

    auto const d = pow(board.size(),2);
    auto const x = (heightG-80)/d;
    
    for (int l = 1; l <= board.size(); ++l)
    {
        for (int j = 1; j <= board.size(); ++j)
        {
            if (board(l, j) == State::Susc)
            {
                ++p.sus;
            }
            if (board(l ,j) == State::Rec)
            {
                ++p.rec;
            }
            if (board(l, j) == State::Inf)
            {
                ++p.inf;
            }
        }
    }

    p1.sus=heightG-50-round(p.sus*x);
    p1.rec=heightG-50-round(p.rec*x);
    p1.inf=heightG-50-round(p.inf*x);

    return p1;
}
//definisco un vector v che dovrà contenere i vari Points, quindi pongo p1 uguale al point che viene fuori
//dalla funzione ser_points, ridefinisco la size di v utilizzando un int che incremento ad ogni ciclo nel main,
//assegno p1 all'ultimo elemento di v (i vari std::cout mi servono per vedere fino a dove mi funge la funzione)
std::vector<Points> v_point (Board const &board, Points &p, int n,std::vector<Points> v)
{
    Points p1;
    p1=set_points(board,p);
    v.push_back(p1);
    return v;
}


//bisogna capire bene come funzionano vertexarray e il primitivetype::Lines
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
        g_window.waitEvent(event);
        while (event.type != sf::Event::KeyPressed)
        {
            if (event.key.code != sf::Keyboard::Escape)
            {
                g_window.waitEvent(event);
            }
        }
    }

    void wait_key_pressed()
    {
        sf::Event event;

        g_window.waitEvent(event);
        while (event.type != sf::Event::KeyPressed)
        {
            if (event.key.code != sf::Keyboard::Escape)
            {
                g_window.waitEvent(event);
            }
        }
    }

    void closing()
    {
        sf::Event e;
        g_window.waitEvent(e);

        while(e.key.code!=sf::Keyboard::E)
        {
            g_window.waitEvent(e);
            
            if(e.key.code==sf::Keyboard::E)
            {
                g_window.close();
            }
        }
    }

    int dra()
    {
        const sf::Vector2f dimXaxis = sf::Vector2f(widthG, 2);
        const sf::Vector2f dimYaxis = sf::Vector2f(2, heightG);
        sf::RectangleShape Xaxis(dimXaxis);
        sf::RectangleShape Yaxis(dimYaxis);
        Xaxis.setPosition(0, heightG - 50);
        Xaxis.setFillColor(sf::Color::Black);
        Yaxis.setPosition(50, 0);
        Yaxis.setFillColor(sf::Color::Black);

        sf::Font font;
        if(!font.loadFromFile("/home/tasso/root/fonts/arial.ttf"))
        {
            throw("COULD NOT LOAD THE FILE");
        }

        std::string n_day="diocane";
        sf::Text text(n_day, font);
        text.setFont(font);
        text.setString(n_day);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::Red);
/*
        g_window.clear(sf::Color::White);

        g_window.draw(Xaxis);
        g_window.draw(Yaxis);

        sf::VertexArray lines(sf::LinesStrip, 4);

        lines[0].position = sf::Vector2f(100, 50);
        lines[0].color=sf::Color::Red;
        lines[1].position = sf::Vector2f(200, 100);
        lines[1].color=sf::Color::Red;
        lines[2].position = sf::Vector2f(300, 50);
        lines[2].color=sf::Color::Red;
        lines[3].position = sf::Vector2f(450, 200);
        lines[3].color=sf::Color::Green;

        g_window.draw(lines);
        */
       g_window.draw(text);
        g_window.display();
    }

    void draw(Board const &board, std::vector<Points> g_points)
    { 
        sf::Font font;
        if(!font.loadFromFile("/home/tasso/root/fonts/arial.ttf"))
        {
            throw("COULD NOT LOAD THE FILE");
        }
        auto const d = pow(board.size(),2);
        auto const x = (heightG-80)/d;
        const sf::Vector2f dimXaxis = sf::Vector2f(widthG, 2);
        const sf::Vector2f dimYaxis = sf::Vector2f(2, heightG);
        sf::RectangleShape Xaxis(dimXaxis);
        sf::RectangleShape Yaxis(dimYaxis);
        Xaxis.setPosition(0, heightG - 50);
        Xaxis.setFillColor(sf::Color::Black);
        Yaxis.setPosition(50, 0);
        Yaxis.setFillColor(sf::Color::Black);

        g_window.clear(sf::Color::White);

        std::string n_day;
        sf::Text text(n_day,font);
        text.setFont(font);
        //text.setString(n_day);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::Black);
        std::vector<int> v_text;

        sf::VertexArray curveS(sf::LineStrip, g_points.size());
        sf::VertexArray curveR(sf::LineStrip, g_points.size());
        sf::VertexArray curveI(sf::LineStrip, g_points.size());

        sf::CircleShape PointS(2);
        sf::CircleShape PointR(2);
        sf::CircleShape PointI(2);

        PointS.setFillColor(sf::Color::Blue);
        PointR.setFillColor(sf::Color::Green);
        PointI.setFillColor(sf::Color::Red);

        for (int a=0; a != g_points.size(); a++)
        {
            std::cout<<a<<'\n';
            double b =a/5-floor(a/5);
            double c= a*0.2;
            std::cout<<floor(c)<<' '<<c<<'\n';
            std::cout <<"numero b ="<<b<<'\n';
            //std::cout<<n_day<<'\n';
            float day=50+round(a*(widthG-50)/g_points.size());
            //float g=50+round(a*(widthG-50)/g_points.size());
            auto point=g_points[a];
            curveS[a].position=sf::Vector2f(day, point.sus);
            PointS.setPosition(day,point.sus-2);
            curveS[a].color=sf::Color::Blue;
            curveR[a].position=sf::Vector2f(day, point.rec);
            PointR.setPosition(day,point.rec-2);
            curveR[a].color=sf::Color::Green;
            curveI[a].position=sf::Vector2f(day, point.inf);
            PointI.setPosition(day,point.inf-2);
            curveI[a].color=sf::Color::Red;
            
            if(floor(c)==c)
            {
                v_text.push_back(a);
            }
            /*
            else
            {
                n_day=" ";
                sf::Vector2f v_text (day+1, heightG-42);
                text.setPosition(v_text);
                text.setString(n_day);  
                g_window.draw(text);
            }
            */
        
            g_window.draw(PointS);
            g_window.draw(PointR);
            g_window.draw(PointI);
        }
        for(int b=0; b!=v_text.size(); b++)
        {
            n_day=std::to_string(v_text[b]);
            float day1= 50+5*round(b*(widthG-50)/g_points.size());
            sf::Vector2f v_text (day1+1, heightG-42);
            text.setPosition(v_text);
            text.setString(n_day);  
            g_window.draw(text);
        }
        /*
        sf::RectangleShape box;
        box.setPosition(heightG/2,widthG/2);
        box.setFillColor(sf::Color::Cyan);
        const sf::Vector2f size (350.f,250.f);
        box.setSize(size);
        g_window.draw(box);
*/
        g_window.draw(curveR);
        g_window.draw(curveI);
        g_window.draw(curveS);
    
        g_window.draw(Xaxis);
        g_window.draw(Yaxis);

        g_window.display();
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

    void closing()
    {
        sf::Event e;
        m_window.waitEvent(e);

        while(e.key.code!=sf::Keyboard::E)
        {
            m_window.waitEvent(e);
            
            if(e.key.code==sf::Keyboard::E)
            {
                m_window.close();
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
        return 0;
    }
};

int main()
{
    int dim = 40;
    int a=1;
    Board board(dim);
    Points point;
    Points p;
    std::vector<Points> g_points;
    p=set_points(board,point);
    g_points=v_point(board, point,a,g_points);

    board(0, 39) = State::Inf;
    board(11, 17) = State::Inf;
    board(2, 2) = State::Inf;
    board(13,16) = State::Inf;
    board(3, 9) = State::Inf;
    board(14, 15) = State::Inf;
    board(14, 39) = State::Inf;

    Display display(dim);
    display.draw(board);
    display.wait_key_pressed();
    while (true)
    {
        a++;
        board = evolve(board, 0.4, 0.4);
        display.draw(board);
        p=set_points(board, point);
        g_points=v_point(board, point, a, g_points);
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        
        if (countI(board) == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1600));
            break;
        }       
    }
    display.closing();

    Graph graph;
    graph.wait_key_pressed();
    //graph.dra();
    //graph.text_instructions();
    graph.draw(board, g_points);
    graph.closing();
   
}
