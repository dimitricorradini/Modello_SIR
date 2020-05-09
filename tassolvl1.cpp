#include <chrono>
#include <iostream>
#include <thread>

float gamma_;
float beta_;

struct State {
  double s_;
  double r_;
  double i_;
};

class Plague {
  int n_;
  State ep_;

public:
  Plague(int n, State const &ep) : n_{n}, ep_{ep} {}

  State CreatePlague(int n) {
    State beg;
    beg.s_ = n - 1;
    beg.r_ = 0;
    beg.i_ = 1;
    return beg;
  };

  void Print(State const &current) {
    std::cout << current.s_ << " " << current.r_ << " " << current.i_ << '\n';
    std::cout << '\n';
  };
};

State Evolve(State const &now, float gamma_, float beta_) {
  State next;

  int N_ = now.s_ + now.r_ + now.i_;
  next.s_ = now.s_ - beta_ * now.i_ * now.s_;
  next.r_ = now.r_ + gamma_ * now.i_;
  next.i_ = now.i_ + beta_ * now.i_ * now.s_ - gamma_ * now.i_;
  
  if (next.s_ > 0) {
  
  if (next.s_ - (int(next.s_)) < 0.6){
    next.s_ = int(next.s_);
  } else {
    next.s_ = int(next.s_ + 1);
  };

  if (next.r_ - (int(next.r_)) < 0.6) {
    next.r_ = int(next.r_);
  } else {
    next.r_ = int(next.r_ + 1);
  };

  if (next.i_ - (int(next.i_)) < 0.6) {
    next.i_ = int(next.i_);
  } else {
    next.i_ = int(next.i_ + 1);
  }

  } else {
    next.i_ = N_ - next.r_;
    next.s_=0;

  if (next.r_ - (int(next.r_)) < 0.6) {
    next.r_ = int(next.r_);
  } else {
    next.r_ = int(next.r_ + 1);
  };

  if (next.i_ - (int(next.i_)) < 0.6) {
    next.i_ = int(next.i_);
  } else {
    next.i_ = int(next.i_ + 1);
  }
  }
  return next;
}

int main() {

  float gamma_ = 0.2;
  float beta_ = 0.1;
  int dim = 10000;
  State s{};
  Plague covid{dim, s};

  s = covid.CreatePlague(dim);
  covid.Print(s);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));
  
  for (int i = 0; i != 15; ++i) {
    s = Evolve(s, gamma_, beta_);
    covid.Print(s);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
  }
}
