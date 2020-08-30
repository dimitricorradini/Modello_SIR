#ifndef SIR_POINTS_HPP
#define SIR_POINTS_HPP

namespace SIR 
{
  class Board;

  static constexpr int heightG = 700;
  static constexpr int widthG = 700;    
    
  struct Points
  {
    int sus;
    int rec;
    int inf;
    int dead;
  };

  Points count(Board const& board);

  Points convert(Board const& board, Points const& point);
}

#endif