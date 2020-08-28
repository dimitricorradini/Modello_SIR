#include "doctest.h"

#include "points.hpp"
#include "board.hpp"

TEST_CASE("Testing points")
{
	
	SIR::Board board(50);
	SIR::Points point;

	points = SIR::count(board);
	int tot = board.side()*board.side();

	CHECK(point.inf == 0);
	CHECK(point.rec == 0);
	CHECK(point.dead == 0);
	CHECK(point.susc == tot);

	SIR::Points point_convert;
  point_convert = SIR::convert(point);

  CHECK(point_convert.inf == 650);
  CHECK(point_convert.rec == 650);
  CHECK(point_convert.dead == 650);
  CHECK(point_convert.susc == 30);


	for (int i = 0; i < tot; i++)
	{
    board[i] = SIR::State::inf;
	}
  
  point = SIR::count(board);

  CHECK(point.inf == tot);
  CHECK(point.rec == 0);
  CHECK(point.dead == 0);
  CHECK(point.susc == 0);

  point_convert = SIR::convert(point);

  CHECK(point_convert.inf == 30);
  CHECK(point_convert.rec == 650);
  CHECK(point_convert.dead == 650);
  CHECK(point_convert.susc == 650);

  for (int i = 0; i < tot; i++)
  {
  	if (i % 2 == 0)
  	{
  	  board[i] = SIR::State::rec;
  	}
  	else
  	{
  		board[i] = SIR::State::dead;
  	}
  }

  point = SIR::count(board);

  CHECK(point.inf == 0);
  CHECK(point.rec == tot / 2);
  CHECK(point.dead == tot / 2);
  CHECK(point.susc == 0);

  point_convert = SIR::convert(point);

  CHECK(point_convert.inf == 650);
  CHECK(point_convert.rec == 340);
  CHECK(point_convert.dead == 340);
  CHECK(point_convert.susc == 650);
    
}