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
  SIR::Display display1(dim);
  display1.show_instructions();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  SIR::Display display(dim);
  display.draw(board);
  display.click(board);
  display.draw(board);

  SIR::Points p = SIR::count(board);

  g_points.push_back(SIR::convert(board, p));

  display.draw(board);
  
  while (true)
  {
    board = SIR::evolve(board, 0.6, 0.4, 0.1);
    display.draw(board);
    p = SIR::count(board);
    g_points.push_back(SIR::convert(board, p));
    std::this_thread::sleep_for(std::chrono::milliseconds(800)); 
    if (p.inf == 0)
    {
      break;
    }
  }

  display.closing();

  SIR::Graph graph;

  graph.draw(board, g_points);
  graph.closing();
  std::cout << "Thanks for playing!" << '\n';
}