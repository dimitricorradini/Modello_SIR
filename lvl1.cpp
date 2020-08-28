#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>

struct State
{
    int S;
    int R;
    int I;
};

class Graph
{
	  private:

    State start_;
    double Beta_;
    double Gamma_;

    public:
    
    Graph(State start, double Beta, double Gamma)
    : start_{start}, Beta_{Beta}, Gamma_{Gamma} {
        if (Beta > 1 || Gamma > 1 || Beta < 0 || Gamma < 0) {
            throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"}; 
        }
    }

    std::vector<State> creategraph() {
        std::vector<State> SRIgraph_{start_};
        double N = start_.S + start_.I + start_.R;
        while (true) {
            auto const& current = SRIgraph_.back();
            State next;
           
            next.S = floor(current.S-Beta_*current.I*current.S);
            next.R = ceil(current.R+Gamma_*current.I);

            if (next.S < 0){ 
              next.S = 0;
            }

            next.I = N - next.R - next.S;           
            assert(next.S+next.I+next.R == N);      
            SRIgraph_.push_back(next);
            if (next.I == 0) { break; }
        }
        return SRIgraph_;
    }   
};

void printraw(std::vector<State> myGraph)
{
	int day = 0;
	for (auto const& state : myGraph) {
    std::cout << "Day "<< day + 1 << "\n";
    std::cout << "Number of susceptible people: " << state.S << "\n";
    std::cout << "Number of infected people: " << state.I << "\n";
    std::cout << "Number of recovered and/or dead people: " << state.R << "\n";
    ++day;
  }
  assert(day == myGraph.size());
}

int main()
{
    State t_0{};
    t_0.S = 1000;
    t_0.I = 1;
    Graph disease(t_0, 0.3, 0.1);
    auto SRIgraph = disease.creategraph();
    printraw(SRIgraph);
  
}