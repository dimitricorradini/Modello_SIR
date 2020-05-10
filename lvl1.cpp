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
            double nextS=current.S-Beta_*current.I*current.S;
            double nextR=current.R+Gamma_*current.I;
            //appprossimo!
            
            if (nextS-floor(nextS)>0.5){
                next.S=ceil(nextS);
            } else {
                next.S=floor(nextS);
            };

            if (nextR-floor(nextR)>=0.5){
                next.R=ceil(nextR);
            } else {
                next.R=floor(nextR);
            };
            
            //what if the virus stops mid-way???
            if (next.S<0){ 
                next.S=0;
            };
            
            next.I=N-next.R-next.S;           
            assert(next.S+next.I+next.R == N);      
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
    t_0.S = 1000;
    t_0.I = 1;
    Graph disease(t_0, 0.3, 0.1);
    auto SRIgraph = disease.creategraph(100);
    printraw(SRIgraph);
  
}
