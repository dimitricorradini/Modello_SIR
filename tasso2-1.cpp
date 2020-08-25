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
    Rec,
    Dead
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

Board evolve(Board const &current, double const& beta, double const& gamma, double const& mu)
{
    // add exception for value of beta, gamma
    if (beta > 1 || (gamma+mu) > 1 || beta < 0 || gamma < 0 || mu < 0)
    {
        throw std::runtime_error{"Coefficient error: inserted values don't make sense"};
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
                double prob_rec = dist(gen);
                if (prob_rec < gamma)
                {
                    //recovery/death
                    next(i, j) = State::Rec;
                }
                else
                {
                    double prob_death = dist(gen);
                    if (prob_death < mu)
                    {
                        next(i, j) = State::Dead;
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
            }
            else
            {
                if (next(i, j) != State::Inf)
                { //tranne inf giacchè non può disinfettarsi in automatico!
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
    float dead;
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


Points set_points(Board const &board)
{
    Points p;
    Points p1;
    p1.inf=0.f;
    p1.rec=0.f;
    p1.sus=0.f;
    p1.dead=0.f;
    p.sus=0.f;
    p.rec=0.f;
    p.inf=0.f;
    p.dead=0.f;

    auto const d = pow(board.size(),2);
    auto const x = (heightG-80)/d;
    
    for (int l = 1; l <= board.size()-1; ++l)
    {
        for (int j = 1; j <= board.size()-1; ++j)
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
            if (board(l, j) == State::Dead)
            {
                ++p.dead;
            }
        }
    }

    p1.sus=heightG-50-round(p.sus*x);
    p1.rec=heightG-50-round(p.rec*x);
    p1.inf=heightG-50-round(p.inf*x);
    p1.dead=heightG-50-round(p.dead*x);

    return p1;
}
//bisogna capire bene come funzionano vertexarray e il primitivetype::Lines
class Graph
{
    sf::RenderWindow g_window;
    static constexpr auto g_window_title = "Trend";
    std::vector<Points> g_points;

public:
    Graph() : g_window{sf::VideoMode(widthG, heightG), g_window_title, sf::Style::Close}, g_points(0) {}

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

        while(e.key.code!=sf::Keyboard::Q)
        {
            g_window.waitEvent(e);
            
            if(e.key.code==sf::Keyboard::Q)
            {
                g_window.close();
            }
        }
    }

    void draw(Board const &board, std::vector<Points> g_points)
    { 
        int r=-1;
        auto const d = pow(board.size(),2);
        auto const x = (heightG-100)/d;
        
        const sf::Vector2f dimXaxis (widthG-55, 2);
        const sf::Vector2f dimYaxis (2, heightG-100);
        const sf::Vector2f arrow (20, 2);
        const sf::Vector2f arrow1 (2,20);
        const sf::Vector2f legend_box(310,45);
        const sf::Vector2f lines(30,2);
        const sf::Vector2f p_line(8,1);
        const sf::Vector2f p_line1(12,2);
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
        lineS.setPosition(widthG-226,38);
        lineR.setPosition(widthG-154,38);
        lineI.setPosition(widthG-72,38);
        lineD.setPosition(widthG-298,38);
        lineD.setFillColor(sf::Color::Black);
        lineS.setFillColor(sf::Color::Blue);
        lineR.setFillColor(sf::Color::Green);
        lineI.setFillColor(sf::Color::Red);
        legend.setPosition(widthG-legend_box.x-2,2);
        legend.setFillColor(sf::Color::White);
        legend.setOutlineThickness(2);
        legend.setOutlineColor(sf::Color::Black);
        y_arrow.setPosition(50,50);
        y_arrow.setFillColor(sf::Color::Black);
        y_arrow.setRotation(45);
        y_arrow1.setPosition(50,50);
        y_arrow1.setRotation(-45);
        y_arrow1.setFillColor(sf::Color::Black);
        x_arrow.setPosition(widthG-18,heightG-64);
        x_arrow.setFillColor(sf::Color::Black);
        x_arrow.setRotation(45);
        x_arrow1.setFillColor(sf::Color::Black);
        x_arrow1.setPosition(widthG-19,heightG-36);
        x_arrow1.setRotation(-45);
        Xaxis.setPosition(50, heightG - 50);
        Xaxis.setFillColor(sf::Color::Black);
        Yaxis.setPosition(50,50);
        Yaxis.setFillColor(sf::Color::Black);

        g_window.clear(sf::Color::White);

        sf::Font font;
        if(!font.loadFromFile("/home/tasso/root/fonts/arial.ttf"))
        {
            throw("COULD NOT LOAD THE FILE");
        }
        sf::Text Xaxis_name("Days",font);
        sf::Text Yaxis_name("Population",font);
        std::string n_day;
        std::string pop_s;
        sf::Text pop(pop_s,font);
        sf::Text sus("Suscptible", font);
        sf::Text rec("Recovered", font);
        sf::Text inf("Infected", font);
        sf::Text dead("Dead", font);
        sf::Text text(n_day,font);
        sf::Text leg("Legend:",font);
        Xaxis_name.setStyle(sf::Text::Bold);
        Xaxis_name.setPosition(widthG-45,heightG-25);
        Xaxis_name.setCharacterSize(15);
        Xaxis_name.setFillColor(sf::Color::Black);
        Yaxis_name.setStyle(sf::Text::Bold);
        Yaxis_name.setPosition(10,30);
        Yaxis_name.setCharacterSize(15);
        Yaxis_name.setFillColor(sf::Color::Black);
        leg.setStyle(sf::Text::Bold);
        leg.setPosition(widthG-306,3);
        leg.setCharacterSize(15);
        leg.setFillColor(sf::Color::Black);
        dead.setStyle(sf::Text::Regular);
        dead.setCharacterSize(13);
        dead.setFillColor(sf::Color::Black);
        dead.setPosition(widthG-298,20);
        sus.setStyle(sf::Text::Regular);
        sus.setCharacterSize(13);
        sus.setFillColor(sf::Color::Blue);
        sus.setPosition(widthG-226,20);
        rec.setStyle(sf::Text::Regular);
        rec.setCharacterSize(13);
        rec.setFillColor(sf::Color::Green);
        rec.setPosition(widthG-154,20);
        inf.setStyle(sf::Text::Regular);
        inf.setCharacterSize(13);
        inf.setFillColor(sf::Color::Red);
        inf.setPosition(widthG-72,20);
        pop.setStyle(sf::Text::Bold);
        pop.setCharacterSize(10);
        pop.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::Black);
        std::vector<int> v_text;
        std::vector<int> v_pop;

        sf::VertexArray curveS(sf::LineStrip, g_points.size());
        sf::VertexArray curveR(sf::LineStrip, g_points.size());
        sf::VertexArray curveI(sf::LineStrip, g_points.size());
        sf::VertexArray curveD(sf::LineStrip, g_points.size());

        sf::CircleShape PointS(2,50);
        sf::CircleShape PointR(2,50);
        sf::CircleShape PointI(2,50);
        sf::CircleShape PointD(2 , 50);

        PointS.setFillColor(sf::Color::Blue);
        PointR.setFillColor(sf::Color::Green);
        PointI.setFillColor(sf::Color::Red);
        PointD.setFillColor(sf::Color::Black);

        for (int a=0; a != g_points.size(); a++)
        {
            std::cout<<a<<'\n';
            //double b =a/5-floor(a/5);
            double c= a*0.2;
            std::cout<<floor(c)<<' '<<c<<'\n';
            //std::cout <<"numero b ="<<b<<'\n';
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

            curveD[a].position=sf::Vector2f(day, point.dead);
            PointD.setPosition(day,point.dead-2);
            curveD[a].color=sf::Color::Black;
            
            if(floor(c)==c)
            {
                v_text.push_back(a);
            }
      
            g_window.draw(PointS);
            g_window.draw(PointR);
            g_window.draw(PointI);
            g_window.draw(PointD);
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
        
        for(int y=0;y<=heightG-100;y=y+(250*x))
        {
            r++;
            sf::Vector2f pos_pop(46,heightG-50-y);
            sf::Vector2f pos_pop1(44,heightG-50-y);
            sf::Vector2f p_pop(20,(heightG-50)-y);
            pop_s=std::to_string(r*250);

            if(r*0.25==floor(r*0.25))
            {
                pop.setString(pop_s);
                pop.setPosition(p_pop);
                pop_line1.setPosition(pos_pop1);
                g_window.draw(pop_line1);
                g_window.draw(pop);
            }
            else if(r*0.25!=floor(r*0.25))
            {
                pop.setString(pop_s);
                pop.setPosition(p_pop);
                pop_line.setPosition(pos_pop);
                g_window.draw(pop_line);
                g_window.draw(pop);
            }
        }
       /*
        std::vector<sf::RectangleShape> v_draw {legend,lineS,lineR,lineI,Yaxis,Xaxis,x_arrow,x_arrow1,y_arrow,y_arrow1};
        for(int x=0; x<=v_draw.size();x++)
        {
            g_window.draw(v_draw[x]);
        }
        
        std::vector<sf::Text> v_text_draw {leg,sus,rec,inf};
        for(int x=0; x<=v_text_draw.size();x++)
        {
            g_window.draw(v_text_draw[x]);
        }
        
        std::vector<sf::VertexArray> v_vertex_array {curveS,curveR,curveI};
        for(int x=0;x<=v_vertex_array.size();x++)
        {
            g_window.draw(v_vertex_array[x]);
        }
        */

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
    bool stop_selection()
    {
        sf::Event event;

        m_window.waitEvent(event);
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            return false;
        }
        else if(event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Escape)
        {
            return true;
        }
        else
        {
            return true;
        }
        
    }
    

    void closing()
    {
        sf::Event e;
        m_window.waitEvent(e);

        while(e.key.code!=sf::Keyboard::Q)
        {
            m_window.waitEvent(e);
            
            if(e.key.code==sf::Keyboard::Q)
            {
                m_window.close();
            }
        }
    }
    void closing_click()
    {
        sf::Event _event;
        m_window.waitEvent(_event);

        while(_event.key.code!=sf::Keyboard::Escape)
        {
            m_window.waitEvent(_event);
            
            if(_event.key.code==sf::Keyboard::Escape)
            {
                std::cout<<'b'<<'\n';
                break;
            }
        }
    }

    void show_instructions(Board const& board)
    {
        const sf::Vector2f dim_box((m_board_side * cell_size)*0.8,(m_board_side * cell_size)*0.6);
        sf::RectangleShape box(dim_box);
        box.setPosition(80,80);
        m_window.clear(sf::Color::White);
        
        sf::Texture t_box;
        t_box.loadFromFile("/tmp/mozilla_tasso0/info_text-2.jpg");
        if((t_box.loadFromFile("/tmp/mozilla_tasso0/info_text-2.jpg")))
        {
            std::cout<<'8'<<'\n';
        }
        
        box.setFillColor(sf::Color::White);
        box.setTexture(&t_box);
        m_window.draw(box);
        m_window.display();

    }

        void click(Board &board)
    {
        std::cout<<'q'<<'\n';
        sf::Vector2i mouse;
        sf::Event e;
        m_window.waitEvent(e);
        while(e.key.code!=sf::Keyboard::Escape)
        {
            while(e.key.code != sf::Mouse::Left && e.key.code!=sf::Keyboard::Escape)
            {
                std::cout<<'s'<<'\n';
                m_window.waitEvent(e);
                if(e.key.code == sf::Mouse::Left)
                {
                    mouse=sf::Mouse::getPosition(m_window);
                    if(mouse.x<0 || mouse.y<0 || mouse.x>(m_board_side*cell_size) ||mouse.y>(m_board_side*cell_size))
                    std::cout<<mouse.x<<' '<<mouse.y<<'\n';
                    for (int i = 0; i != m_board_side; ++i)
                    {
                        std::cout<<"o ";
                        if((i*10)<mouse.x && ((i+1)*10)>mouse.x)
                        {
                            std::cout<<'1'<<'\n';
                            for (int j = 0; j != m_board_side; ++j)
                            {
                                if((j*10)<mouse.y && ((j+1)*10)>mouse.y)
                                {
                                    std::cout<<'2'<<'\n';
                                    board(i,j)=State::Inf;
                                    draw(board);
                                    if(board(i,j)==State::Inf)
                                    {
                                        std::cout<<'3'<<'\n';
                                    }
                                }
                            }
                        }
                    }
                }
            }
            std::cout<<'z'<<'\n';
        }  
    }

    
    
    int draw(Board const &board)
    {
        const sf::Vector2f cell_vector = sf::Vector2f(cell_size, cell_size);
        sf::RectangleShape cell1(cell_vector);
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
                if(board(i,j)== State::Dead)
                {
                    cell.setFillColor(sf::Color::Black);
                }
                m_window.draw(cell);
                
            }
        }
        for (int i = 0; i != m_board_side; ++i)
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
        }
        m_window.display();
        return 0;
    }
};

int main()
{
    int dim = 40;
    int b = 15;
    int a;
    Board board(dim);
    std::vector<Points> g_points;
    Display display1(dim);
    display1.show_instructions(board);
    display1.closing();
    Display display(dim);
    display.draw(board);
    //std::cout<<countS(board)<<'\n';
    //display.draw_info();
    //display.draw(board);
    //for(a=0; a!=b; a++)

        display.click(board);
        //std::cout<<"click fatta"<<'\n';
        /*
        display.draw(board);
        std::cout<<"draw fatta"<<'\n';
*/


    /*
    board(0, 39) = State::Inf;
    board(11, 17) = State::Inf;
    board(2, 2) = State::Inf;
    board(13,16) = State::Inf;
    board(3, 9) = State::Inf;
    board(14, 15) = State::Inf;
    board(25, 39) = State::Inf;
    board(1, 3) = State::Inf;
    board(16, 9) = State::Inf;
    board(27, 25) = State::Inf;
    board(2, 4) = State::Inf;
    */
    
    //std::cout<<countS(board)<<'\n';
    
    g_points.push_back(set_points(board));
    
    display.wait_key_pressed();
    display.draw(board);

    while (true)
    {
        a++;
        board = evolve(board, 0.8, 0.2,0.5);
        display.draw(board);
        //std::cout<<countS(board)<<'\n';
        g_points.push_back(set_points(board));
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
