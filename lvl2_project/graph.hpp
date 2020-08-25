#ifndef SIR_GRAPH_HPP
#define SIR_GRAPH_HPP

#include <SFML/Graphics.hpp>
#include "points.hpp"
#include <cmath>
#include <string>

namespace SIR {

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

    void draw(std::vector<Points>& g_points)
    { 

        const sf::Vector2f dimXaxis (widthG, 2);
        const sf::Vector2f dimYaxis (2, heightG);
        sf::RectangleShape Xaxis(dimXaxis);
        sf::RectangleShape Yaxis(dimYaxis);
        Xaxis.setPosition(0, heightG - 50);
        Xaxis.setFillColor(sf::Color::Black);
        Yaxis.setPosition(50, 0);
        Yaxis.setFillColor(sf::Color::Black);

        g_window.clear(sf::Color::White);

        sf::Font font;
        if(!font.loadFromFile("/home/tasso/root/fonts/arial.ttf"))
        {
            throw("COULD NOT LOAD THE FILE");
        }
        std::string n_day;
        sf::Text text(n_day,font);
        text.setFont(font);
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

        for (unsigned int a=0; a != g_points.size(); a++)
        {
            float day=50+round(a*(widthG-50)/g_points.size());
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
            
            if(a%5==0)
            {
                v_text.push_back(a);
            }

            g_window.draw(PointS);
            g_window.draw(PointR);
            g_window.draw(PointI);
        }
        for(unsigned int b=0; b!=v_text.size(); b++)
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

}

#endif
