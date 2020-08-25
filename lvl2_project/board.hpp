#ifndef SIR_BOARD_HPP
#define SIR_BOARD_HPP

#include <cassert>
#include <vector>
#include <random>

namespace SIR{

enum class State
{
    Susc,
    Inf,
    Rec
};

class Board
{
private:
    int m_side;
    std::vector<State> m_board;

public:
    Board(int side) : m_size(side), board_(side * side) {}

    State operator()(int i, int j) const
    {
        return (i >= 0 && i < n_ && j >= 0 && j < n_) ? board_[i * n_ + j] : State::Rec;
    }

    State &operator()(int i, int j)
    {
        assert(i >= 0 && i < n_ && j >= 0 && j < n_);
        return board_[i * n_ + j];
    }

    int size() const { return m_side; }
};

inline Board evolve(Board const &current, double const beta, double const gamma)
{
    // add exception for value of beta, gamma
    if (beta > 1 || gamma > 1 || beta < 0 || gamma < 0)
    {
        throw std::runtime_error{"Coefficients Beta and Gamma must be between 0 and 1"};
    }

    int n = current.size();
    Board next(n);
    //seed for random generation
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dist{0, 1};
    //cycle on matrix
    for (int i = 0; i != n; ++i)
    {
        for (int j = 0; j != n; ++j)
        {
            if (current(i, j) == State::Inf)
            {
                double prob1 = dist(gen);
                if (prob1 < gamma)
                {
                    //recovery/death
                    next(i, j) = State::Rec;
                }
                else
                {
                    next(i, j) = State::Inf;
                    //cycle on neighbors
                    for (int l = i - 1; l <= i + 1; ++l)
                    {
                        for (int m = j - 1; m <= j + 1; ++m)
                        {
                            double prob2 = dist(gen);
                            if (current(l, m) == State::Susc && prob2 < beta)
                            {
                                //infection!
                                next(l, m) = State::Inf;
                            }
                        }
                    }
                }
            }
            else
            {
                if (next(i, j) != State::Inf)
                { //non può disinfettarsi in automatico!
                    next(i, j) = current(i, j);
                }
            }
        }
    }
    return next;
};

}
#endif
