#include "doctest.h"

#include "board.hpp"
#include "points.hpp"

TEST_CASE("Testing board")
{
	CHECK_THROWS(SIR::Board(0));
	CHECK_THROWS(SIR::Board(-30));
  
  SIR::Board const board1(50);
  SIR::Board board2(40);

  int tot1 = board1.side() * board1.side();
  CHECK(tot1 == 2500);

  int tot2 = board2.side() * board2.side();
  CHECK(tot2 == 1600);

  CHECK(board1(0, tot1) == SIR::State::Rec);
  CHECK(board1(0, 4) == SIR::State::Susc);

  board2(1, 5) = SIR::State::Inf;
  CHECK(board2(1, 5) == SIR::State::Inf);
  
  SIR::Points point;

  point = SIR::count(board2);

  while (point.sus != 0)
  {
  	board2 = SIR::evolve(board2, 1, 0, 0);
  	point = SIR::count(board2);
  }
  
  CHECK(point.inf == tot2);
  CHECK(point.rec == 0);
  CHECK(point.dead == 0);
  CHECK(point.sus == 0);


  board2 = SIR::evolve(board2, 0, 1, 0);
  point = SIR::count(board2);
  
  CHECK(point.inf == 0);
  CHECK(point.rec == tot2);
  CHECK(point.dead == 0);
  CHECK(point.sus == 0);

  CHECK_THROWS(SIR::evolve(board2, 2, 0, 0));
  CHECK_THROWS(SIR::evolve(board2, 0, 1.1, 0));

  for (int i = 0; i < board2.side(); i++)
  {
    for (int j = 0; j < board2.side(); j++)
    {
      board2(i, j) = SIR::State::Inf;
    }
  }
  
  board2 = SIR::evolve(board2, 0, 0, 1);
  point = SIR::count(board2);
  
  CHECK(point.inf == 0);
  CHECK(point.rec == 0);
  CHECK(point.dead == tot2);
  CHECK(point.sus == 0);

  for (int i = 0; i < board2.side(); i++)
  {
    for (int j = 0; j < board2.side(); j++)
    {
      board2(i, j) = SIR::State::Susc;
    }
  }

  board2(0, 5) = SIR::State::Inf;
  point = SIR::count(board2);

  while (point.inf != 0)
  {
  	board2 = SIR::evolve(board2, 0.8, 0.8, 0);
  	point = SIR::count(board2);
  }
 
  CHECK(point.dead == 0);
  CHECK(point.sus == tot2-point.rec);

  for (int i = 0; i < board2.side(); i++)
  {
    for (int j = 0; j < board2.side(); j++)
    {
      board2(i, j) = SIR::State::Susc;
    }
  }

  board2(3, 7) = SIR::State::Inf;
  point = SIR::count(board2);

  while (point.inf != 0)
  {
  	board2 = SIR::evolve(board2, 0.8, 0, 0.8);
  	point = SIR::count(board2);
  }
 
  CHECK(point.rec == 0);
  CHECK(point.sus == tot2-point.dead);
}