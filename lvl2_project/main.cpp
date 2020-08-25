#include <chrono>
#include <thread>
#include <vector>
#include "board.hpp"
#include "display.hpp"
#include "points.hpp"
#include "graph.hpp"

int main()
{
    int dim = 40;
    SIR::Board board(dim);
    std::vector<SIR::Points> g_points;

    board(0, 39) = SIR::State::Inf;
    board(11, 17) = SIR::State::Inf;
    board(2, 2) = SIR::State::Inf;
    board(13,16) = SIR::State::Inf;
    board(3, 9) = SIR::State::Inf;
    board(14, 15) = SIR::State::Inf;
    board(14, 39) = SIR::State::Inf;

    g_points.push_back(SIR::set_points(board));

    SIR::Display display(dim);
    display.draw(board);
    display.wait_key_pressed();
    while (true)
    {
        board = evolve(board, 0.4, 0.4);
        display.draw(board);
        g_points.push_back(SIR::set_points(board));
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        
        if (g_points.back().inf == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1600));
            break;
        }       
    }
    display.closing();

    SIR::Graph graph;
    graph.wait_key_pressed();
    //graph.dra();
    //graph.text_instructions();
    graph.draw(g_points);
    graph.closing();
}
