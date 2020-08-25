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
  Board board(dim);
  std::vector<Points> g_points;
  Display display1(dim);
  display1.show_instructions(board);
  display1.closing();
  Display display(dim);
  display.draw(board);
  display.click(board);

  g_points.push_back(set_points(board));
    
  display.wait_key_pressed();
  display.draw(board);

  while (true)
  {
    board = evolve(board, 0.8, 0.2,0.5);
    display.draw(board);
    g_points.push_back(set_points(board));
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    if (g_points.back().inf == 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1600));
      break;
    }       
  }
    
  display.closing();

  Graph graph;
  graph.wait_key_pressed();
  graph.draw(board, g_points);
  graph.closing();
}

