#include "doctest.h"

#include "board.hpp"
#include "point.hpp"

TEST_CASE("Testing board")
{
	CHECK_THROWS(SIR::Board empty_board);
	CHECK_THROWS(SIR::Board impossible_board(-30));
  
  SIR::Board board1(50);
  SIR::Board board2(40);

  int tot1 = board1.side() * board1.side();
  CHECK(tot == 2500);

  int tot2 = board2.side() * board2.side();
  CHECK(tot2 == 1600);

  CHECK(board1(0, tot) == SIR::State::rec);
  CHECK(board1(0, 4) == SIR::State::susc);

  board1[201] = SIR::State::inf;
  CHECK(board1(1, 4) == SIR::State::inf);

  board2[0] = SIR::State::inf;
  CHECK(board2(0, 0) == SIR::State::inf);
  
  SIR::Points point;

  while (point.susc != 0)
  {
  	SIR::evolve(board1, 1, 0, 0);
  	point = SIR::count(board1);
  }
  
  CHECK(point.inf == tot1);
  CHECK(point.rec == 0);
  CHECK(point.dead == 0);
  CHECK(point.susc == 0);


  SIR::evolve(board1, 0, 1, 0);
  point = SIR::count(board1);
  
  CHECK(point.inf == 0);
  CHECK(point.rec == tot1);
  CHECK(point.dead == 0);
  CHECK(point.susc == 0);

  CHECK_THROWS(SIR::evolve(board1, 2, 0, 0));
  CHECK_THROWS(SIR::evolve(board1, 0, 1.1, 0));

  for (int i = 0; i < tot1; i++)
  {
  	board1[i] = SIR::State::inf;
  }
  
  SIR::evolve(board1, 0, 0, 1);
  point = SIR::count(board1);
  
  CHECK(point.inf == 0);
  CHECK(point.rec == 0);
  CHECK(point.dead == tot1);
  CHECK(point.susc == 0);
  
  while (point.inf != 0)
  {
  	SIR::evolve(board2, 0.8, 0.8, 0);
  	point = SIR::count(board2);
  }
 
  CHECK(point.dead == 0);
  CHECK(point.susc == tot2-point.rec);

  for (int i = 1; i < tot2; i++)
  {
  	board1[i] = SIR::State::susc;
  }

  while (point.inf != 0)
  {
  	SIR::evolve(board2, 0.8, 0, 0.8);
  	point = SIR::count(board2);
  }
 
  CHECK(point.rec == 0);
  CHECK(point.susc == tot2-point.dead);
  
}