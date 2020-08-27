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

  g_points.push_back(SIR::set_points(board));

  display.draw(board);

  
  while (true)
  {
    board = SIR::evolve(board, 0.4, 0.2, 0.6);
    display.draw(board);
    g_points.push_back(SIR::set_points(board));
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    if (SIR::countI(board)==0)
    {
      break;
    }
  }

  display.closing();

  SIR::Graph graph;
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  graph.wait_key_pressed();
  graph.draw(board, g_points);
  graph.closing();
  std::cout<<"Thanks for playing!!"<<'\n';
}

