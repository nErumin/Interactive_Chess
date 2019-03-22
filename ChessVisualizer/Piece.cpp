#include <exception>
#include <iterator>
#include <algorithm>

#include "Piece.h"
#include "MovementStrategy.h"
#include "Vector2.h"

Piece::Piece(PieceColor initColor)
    : color{ initColor },
      rotationDegree{ 0.0 }
{
}

std::vector<Vector2> Piece::movableLocations(const Vector2& pieceLocation) const
{
    std::vector<Vector2> locations;

    for (const auto& strategy : movementStrategies)
    {
        if (!isStrategyAcceptable(strategy))
        {
            throw std::logic_error{ "contains non-acceptable strategy" };
        }

        auto movables = strategy->movableLocations(pieceLocation);

        std::copy(movables.begin(), movables.end(), std::back_inserter(locations));
    }

    return locations;
}

std::vector<Vector2> Piece::movableLocationsUsingObstacles(const Vector2& pieceLocation, const std::vector<std::vector<PieceColor>>& obstacleMap) const
{
    std::vector<Vector2> locations;

    for (const auto& strategy : movementStrategies)
    {
        if (!isStrategyAcceptable(strategy))
        {
            throw std::logic_error{ "contains non-acceptable strategy" };
        }

        std::vector<std::deque<bool>> movementObstacleMap{ obstacleMap.size(), std::deque<bool>(obstacleMap.size(), false) };
        for (size_t i = 0; i < movementObstacleMap.size(); ++i)
        {
            for (size_t j = 0; j < movementObstacleMap[i].size(); ++j)
            {
                movementObstacleMap[i][j] = obstacleMap[i][j] == PieceColor::None ?
                                            false :
                                            true;
            }
        }

        auto obstacleMovablesPair = strategy->movableLocationsUsingObstacles(pieceLocation, movementObstacleMap);

        std::copy(obstacleMovablesPair.second.begin(), obstacleMovablesPair.second.end(), std::back_inserter(locations));
        std::copy_if(obstacleMovablesPair.first.begin(), obstacleMovablesPair.first.end(), std::back_inserter(locations),
                     [this, &obstacleMap](const Vector2& obstacleLocation)
        {
            auto normalized = normalizeToIntegerVector(obstacleLocation);
            return obstacleMap[normalized.first][normalized.second] != getColor();
        });
    }

    return locations;
}


double Piece::getRotationDegree() const noexcept
{
    return rotationDegree;
}

PieceColor Piece::getColor() const noexcept
{
    return color;
}

void Piece::setRotationDegree(double newDegree) noexcept
{
    rotationDegree = newDegree;
}

void Piece::setColor(PieceColor newColor) noexcept
{
    color = newColor;
}

void Piece::setMovementStratgies(std::vector<std::unique_ptr<MovementStrategy>>&& newStrategies)
{
    movementStrategies.clear();

    for (auto& strategy : newStrategies)
    {
        movementStrategies.push_back(std::move(strategy));
    }
}

Piece::~Piece()
{

}
