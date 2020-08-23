#ifndef SIR_POINTS_HPP
#define SIR_POINTS_HPP

#include "board.hpp"
#include <vector>
#include <cmath>


namespace SIR{

struct Points
{
    float sus;
    float rec;
    float inf;
};

inline Points set_points(Board const &board)
{
    Points p;
    Points p1;
    p1.inf=0.f;
    p1.rec=0.f;
    p1.sus=0.f;
    p.sus=0.f;
    p.rec=0.f;
    p.inf=0.f;

    auto const d = pow(board.size(),2);
    auto const x = (heightG-80)/d;
    
    for (int l = 1; l <= board.size(); ++l)
    {
        for (int j = 1; j <= board.size(); ++j)
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
        }
    }

    p1.sus=heightG-50-round(p.sus*x);
    p1.rec=heightG-50-round(p.rec*x);
    p1.inf=heightG-50-round(p.inf*x);

    return p1;
};

}

#endif