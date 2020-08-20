#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <tgmath.h>
#include <random>

int constexpr boardwidth = 1000;
enum class State {susc, inf, rec};


/*class Board 
{
  int n_;
  std::vector<State> board_;

  public:
  Board(int n) : n_(n), board_(n * n) {}

  State operator()(int i, int j) const {
    return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j] : State::Rec;
  }

  State& operator()(int i, int j) {
    assert(i >= 0 && i < n_ && j >= 0 && j < n_);
    return board_[i * n_ + j];
  }

  int size() const { return n_; }
};*/

class Person{
  int pos_;
  std::array<int, 2> spd_; 
  State pst_;
    public:
    Person(int pos, State pst, std::array<int, 2> spd) : pos_(pos), pst_(pst), spd_(spd) {
      if (pos<0||pos_>pow(boardwidth,2)-1){
        throw std::runtime_error(
          "Position is out of the board");
      }
      if (spd_[0]<-3||spd_[0]>3||spd[1]<-3||spd[1]>3){
        throw std::runtime_error(
          "People are going too fast!");
      }
    }
    //position must be less than width^2
    //speed between -3 and 3
    int &position(){
        return pos_;
        };
    State state() const{
        return pst_;
    };
    State &state(){
        return pst_;
        };
    auto& speed(){
      return spd_;
    }
    auto speed() const {
        return spd_;
    };
};

//classe population? cosa potrei metterci però tipo suscnumber??
 float d(Person a, Person b){
  int ax = a.position() % boardwidth;
  int ay = (a.position() - ax) / boardwidth;
  int bx = b.position() % boardwidth;
  int by = (b.position() - bx) / boardwidth;
  return sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
 }
 /*void walk(std::vector<person> population){
  for one : current {
    one.position()=one.position()+one.speed()[0]+one.speed()[1]*boardwidth
  }
 }*/
 std::vector<Person> evolve(std::vector<Person> const& current, int beta, int gamma){
  if (beta > 1 || gamma > 1 || beta < 0 || gamma < 0) {
    throw std::runtime_error(
        "Coefficients Beta and Gamma must be between 0 and 1");
  };
  std::vector<Person> next(current.begin(), current.end());
  //devo inizializzarlo con il numero di persone?
  //walk should be here(?)
  auto w_next = next.begin();
  for (auto one:current){
      if (one.position()<3000||one.position()>996999){
          one.speed()[1]=-one.speed()[1];
      }
      if ((one.position()%boardwidth<3)||(997<one.position()%boardwidth)){
          one.speed()[1]=-one.speed()[1];
      }
      (*w_next).position()=one.position()+one.speed()[0]+one.speed()[1]*boardwidth;
      if ((*w_next).position()<0||(*w_next).position()>pow(boardwidth,2)-1){
        throw std::runtime_error(
          "Position is out of the board");
      };
      w_next++;
  }
  auto nit = next.begin();
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> dist{0, 1};

  for (auto& one : current) {
        if (one.state()==State::inf){//randomly generate gammac
          double gammac = dist(gen);
          if (gammac<gamma){
            (*nit).state() = State::rec;
        }
        if (one.state() == State::inf){
            for (std::vector<Person>::iterator i=next.begin(); i!=next.end(); ++i){
              //randomly generate betac
              if ((*i).state()==State::susc){
                if (d(one, *i) < 100){
                  double betac = dist(gen);
                  if (betac<beta){
                    (*i).state()=State::inf;
                    }
                  }  
                }  
              }
            }
        }
      nit++;
  }
  return next;
 }

 int main(){
  std::vector<Person> mypop;
  std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<int> dist{0, 999999};
  for (int a=0; a<=5; a++){
    auto rpos = dist(gen);
    std::array<int, 2> rspd;
    rspd[0]=dist(gen)%6-3;
    rspd[1]=dist(gen)%6-3; 
    Person person(rpos, State::susc, rspd);
    mypop.push_back(person);
    //etc con susc. e inf.
  }
  for (int a=0; a<=100; a++){
    auto rpos = dist(gen);
    std::array<int, 2> rspd;
    rspd[0]=dist(gen)%6-3;
    rspd[1]=dist(gen)%6-3; 
    Person person(rpos, State::inf, rspd);
    mypop.push_back(person);
    //etc con susc. e inf.
  }
  for (int a=0; a<=1000; a++){ 
    mypop=evolve(mypop, 0.5, 0.8);
      int s=0;
      int i=0;
      int r=0;
    for (auto& one: mypop){

      if (one.state()==State::inf){
        //std::cout<<"I \n";
        i++;
      }
      if (one.state()==State::rec){
          //std::cout<<"R \n";
        r++;
      }
      if (one.state()==State::susc){
          //std::cout<<"S \n";
          s++;
      }
      std::cout<<one.position()<<"\n";
      std::cout<<one.speed()[0]<< " "<<one.speed()[1]<<"/n";

    }
    std::cout<<s<<" "<<r<<" "<<i;
    std::cout<<"\n \n \n";
  }
 }
