#ifndef SIR_BOARD_HPP
#define SIR_BOARD_HPP

#include <vector>
#include <cassert>
#include <iostream>

namespace SIR 
{
  enum class State
  {
    Susc,
    Inf,
    Rec,
    Dead
  };

  class Board
  {
    private:
    int m_side;
    std::vector<State> m_board;

    public:
    Board(int side) : m_side(side), m_board(side * side) 
    {
      if (side <= 0)
      {
        throw std::runtime_error{"There is no population"};
      } 
    }

    State operator()(int i, int j) const
    {
      return (i >= 0 && i < m_side && j >= 0 && j < m_side) ? m_board[i * m_side + j] : State::Rec;
    }

    State& operator()(int i, int j)
    {
      assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
      return m_board[i * m_side + j];
    }

    int side() const { return m_side; }
  };

  Board evolve(Board const& current, double const beta, double const gamma, double const mu);
}

#endif

