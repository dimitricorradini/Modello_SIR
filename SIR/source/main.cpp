#include "board.hpp"
#include "display.hpp"
#include "graph.hpp"
#include "info.hpp"
#include "points.hpp"
#include <chrono>
#include <thread>

int main() {
  int dim = 0;
  double infection_rate;
  double recovery_rate;
  double mortality;
  char choice;
  while (dim > 50 || dim < 1) {
    std::cout << '\n' << "Insert board dimension (1 to 50): ";
    std::cin >> dim;
  }
  while (choice != 'S' && choice != 'C') {
    std::cout
        << '\n'
        << "Press S to start the simulation or C to choose your parameters: ";
    std::cin >> choice;
    choice = toupper(choice);
    switch (choice) {
    case ('S'): {
      infection_rate = 0.6;
      recovery_rate = 0.4;
      mortality = 0.1;
      std::cout << '\n' << "Enjoy!" << '\n';
      std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    } break;
    case ('C'): {
      std::cout << '\n';
      std::cout << "Insert infection rate (0 to 1): ";
      std::cin >> infection_rate;
      std::cout << "Insert recovery rate (0 to 1): ";
      std::cin >> recovery_rate;
      std::cout << "Insert mortality (0 to 1): ";
      std::cin >> mortality;
      std::cout << '\n' << "Enjoy!" << '\n';
      std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    } break;

    default: { std::cout << '\n' << "Please type C or S!" << '\n'; } break;
    }
  }

  SIR::Board board(dim);
  std::vector<SIR::Points> g_points;
  SIR::Info info{};
  info.show_instructions();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  SIR::Display display(dim);
  display.draw(board);
  display.click(board);
  display.draw(board);

  SIR::Points p = SIR::count(board);

  g_points.push_back(SIR::convert(board, p));

  display.draw(board);

  while (p.inf != 0) {
    board = SIR::evolve(board, infection_rate, recovery_rate, mortality);
    display.draw(board);
    p = SIR::count(board);
    g_points.push_back(SIR::convert(board, p));
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }

  display.closing();

  SIR::Graph graph;

  graph.draw(board, g_points);
  graph.closing();
  std::cout << '\n' << "Thanks for playing!" << '\n';
}
