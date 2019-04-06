#ifndef GAMERESULT_H
#define GAMERESULT_H

#include <string>

enum class GameResult
{
    None = 0,
    Win = 1,
    Draw = 2,
    Lose = 4
};

std::string toLowerString(GameResult result)
{
    switch (result)
    {
        case GameResult::Win:
        {
            return "win";
        }
        case GameResult::Draw:
        {
            return "draw";
        }
        case GameResult::Lose:
        {
            return "lose";
        }
        default:
        {
            throw std::logic_error{ "not supported" };
        }
    }
}

#endif // GAMERESULT_H
