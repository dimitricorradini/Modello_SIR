#include "doctest.h"

#include "../source/board.hpp"
#include "../source/points.hpp"

TEST_CASE("Testing points") {

  SIR::Board board(50);
  SIR::Points point;

  point = SIR::count(board);
  int tot = board.side() * board.side();

  CHECK(point.inf == 0);
  CHECK(point.rec == 0);
  CHECK(point.dead == 0);
  CHECK(point.sus == tot);

  SIR::Points point_convert;
  point_convert = SIR::convert(board, point);

  CHECK(point_convert.inf == 650);
  CHECK(point_convert.rec == 650);
  CHECK(point_convert.dead == 650);
  CHECK(point_convert.sus == 50);

  for (int i = 0; i < board.side(); i++) {
    for (int j = 0; j < board.side(); j++) {
      board(i, j) = SIR::State::Inf;
    }
  }

  point = SIR::count(board);

  CHECK(point.inf == tot);
  CHECK(point.rec == 0);
  CHECK(point.dead == 0);
  CHECK(point.sus == 0);

  point_convert = SIR::convert(board, point);

  CHECK(point_convert.inf == 50);
  CHECK(point_convert.rec == 650);
  CHECK(point_convert.dead == 650);
  CHECK(point_convert.sus == 650);

  for (int i = 0; i < board.side(); i++) {
    for (int j = 0; j < board.side(); j++) {
      if (i % 2 == 0) {
        board(i, j) = SIR::State::Rec;
      } else {
        board(i, j) = SIR::State::Dead;
      }
    }
  }

  point = SIR::count(board);

  CHECK(point.inf == 0);
  CHECK(point.rec == tot / 2);
  CHECK(point.dead == tot / 2);
  CHECK(point.sus == 0);

  point_convert = SIR::convert(board, point);

  CHECK(point_convert.inf == 650);
  CHECK(point_convert.rec == 350);
  CHECK(point_convert.dead == 350);
  CHECK(point_convert.sus == 650);
}
