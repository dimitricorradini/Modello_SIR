#ifndef SIR_POINTS_HPP
#define SIR_POINTS_HPP

namespace SIR {
class Board;

constexpr int heightG = 700;
constexpr int widthG = 700;

struct Points {
  int sus = 0;
  int rec = 0;
  int inf = 0;
  int dead = 0;
};

Points count(Board const &board);

Points convert(Board const &board, Points const &point);
} // namespace SIR

#endif
