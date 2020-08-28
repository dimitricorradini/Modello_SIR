#include <vector>
#include <cmath>
#include "points.hpp"
#include "board.hpp"

SIR::Points SIR::count(SIR::Board const &board)
{
	Points p;
	p.sus = 0.f;
	p.rec = 0.f;
	p.inf = 0.f;
	p.dead = 0.f;
		
	for (int l = 1; l <= board.side()-1; ++l)
	{
		for (int j = 1; j <= board.side()-1; ++j)
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
	return p;
}

SIR::Points SIR::convert(SIR::Board const& board, Points const& point)
{
	Points p_graph;
	p_graph.inf = 0.f;
	p_graph.rec = 0.f;
	p_graph.sus = 0.f;
	p_graph.dead = 0.f;

	double const d = board.side() * board.side();
	double const x = (heightG - 80) / d;

	p_graph.sus = heightG - 50 - round(point.sus*x);
	p_graph.rec = heightG - 50 - round(point.rec*x);
	p_graph.inf = heightG - 50 - round(point.inf*x);
	p_graph.dead = heightG - 50 - round(point.dead*x);

	return p_graph;
}
