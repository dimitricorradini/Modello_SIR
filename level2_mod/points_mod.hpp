#ifndef SIR_POINTS_HPP
#define SIR_POINTS_HPP

#include "board_mod.hpp"
#include <vector>
#include <cmath>

//namespace SIR 
//{
  static constexpr int heightG = 700;
  static constexpr int widthG = 700;    
    
  struct Points
  {
    float sus;
    float rec;
    float inf;
    float dead;
  };

  int countI(Board const &board)
  {
    int inf_ = 0;
    for (int l = 1; l <= board.size()-1; ++l)
    {
      for (int j = 1; j <= board.size()-1; ++j)
      {
        if (board(l, j) == State::Inf)
        {
          ++inf_;
        }
      }
    }
    return inf_;
  }

  inline Points set_points(Board const &board)
  {
    Points p;
    Points p_graph;
    p_graph.inf = 0.f;
    p_graph.rec = 0.f;
    p_graph.sus = 0.f;
    p_graph.dead = 0.f;
    p.sus = 0.f;
    p.rec = 0.f;
    p.inf = 0.f;
    p.dead = 0.f;

    auto const d = board.size() * board.size();
    auto const x = (heightG-80)/d;
    
    for (int l = 1; l <= board.size()-1; ++l)
    {
      for (int j = 1; j <= board.size()-1; ++j)
      {
        if (board(l, j) == State::Susc)
        {
          ++p.sus;
        }
        if (board(l ,j) == State::Rec)
        {
          ++p.rec;
        }
        if (board(l, j) == State::Inf)
        {
          ++p.inf;
        }
        if (board(l, j) == State::Dead)
        {
          ++p.dead;
        }
      }
    }

    p_graph.sus = heightG-50-round(p.sus*x);
    p_graph.rec = heightG-50-round(p.rec*x);
    p_graph.inf = heightG-50-round(p.inf*x);
    p_graph.dead = heightG-50-round(p.dead*x);

    return p_graph;
  }
//}

#endif
