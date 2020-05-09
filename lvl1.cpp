#include <iostream>
#include <vector>
#include <cassert>

struct State
{
    double S;
    double R;
    double I;
};

class Graph{

    State start_;
    double Beta_;
    double Gamma_;
    //else a State vector and an int(?)
    public:
    
    Graph(State start, double Beta, double Gamma)
    : start_{start}, Beta_{Beta}, Gamma_{Gamma}{
    //gamma, beta must be below one
        if (Beta>1||Gamma>1||Beta<0||Gamma<0) {
            throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"}; 
        };
    };

    std::vector<State> creategraph(int sim_length) {
        std::vector<State> SRIgraph_{start_};
        double N = start_.S+start_.I+start_.R;
        for (int i=0; i!= sim_length; i++) {
            auto const& current = SRIgraph_.back();
            State next;
            next.S=current.S-Beta_*current.I*current.S;
            next.I=current.I+Beta_*current.I*current.S-Gamma_*current.I;
            next.R=current.R+Gamma_*current.I;
            //what if the virus stops mid-way??? Mi sa per questo veniva sottozero a te Tasso
            if (next.S<0){
                next.I=N-next.R;
                next.S=0;
            };
            //assert(next.S+next.I+next.R == N); doesn't work because approximation
            //how to treat doubles vs integers??
            SRIgraph_.push_back(next);
        }


        return SRIgraph_;
    };   
};
void printraw(std::vector<State> myGraph){ //must improve
    int day = 1;
    	for (auto const& state : myGraph){
            std::cout << "Day "<< day << "\n";
    	    std::cout << "Number of susceptible people: " << state.S << "\n";
            std::cout << "Number of infected people: " << state.I << "\n";
            std::cout << "Number of recovered and/or dead people: " << state.R << "\n";
            day++;
    	};
    assert(day==myGraph.size());
};


int main(){
    //definisco stato a t_0
    State t_0{};
    t_0.S = 110;
    t_0.I = 1;
    Graph disease(t_0, 0.5, 0.8);
    auto SRIgraph = disease.creategraph(100);
    printraw(SRIgraph);
  
}

