#ifndef SIR_POINTS_HPP
#define SIR_POINTS_HPP

//#include "board.hpp"

namespace SIR 
{
  class Board;

  static constexpr int heightG = 700;
  static constexpr int widthG = 700;    
    
  struct Points
  {
    float sus;
    float rec;
    float inf;
    float dead;
  };

  Points count(Board const &board);

  Points convert(Board const& board, Points const& point);
}

#endif
