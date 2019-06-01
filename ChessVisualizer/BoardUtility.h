#ifndef BOARDUTILITY_H
#define BOARDUTILITY_H

#include "Vector2.h"
#include "Board.h"
#include "PieceColor.h"
#include <utility>
#include <map>
#include <memory>
#include "Piece.h"
#include "King.h"

inline std::pair<Vector2, Vector2> randomPickPieceMoving(const Board& board, PieceColor playerColor, bool isPassivePick)
{
    std::vector<std::pair<Vector2, std::vector<Vector2>>> movablePieces;
    std::vector<Vector2> opponentLocations;

    try
    {
        // find movable pieces, then map them to movable locations.
        for (size_t i = 0; i < boardSize; ++i)
        {
            for (size_t j = 0; j < boardSize; ++j)
            {
                Vector2 locationVector{ static_cast<double>(j), static_cast<double>(i) };

                auto piecePtr = board.getCell(i, j).getPiece();

                if (piecePtr->getColor() == playerColor)
                {
                    auto movableLocations = board.findPieceMovableLocations(locationVector, playerColor);

                    if (movableLocations.size() > 0)
                    {
                        movablePieces.emplace_back(locationVector, std::move(movableLocations));
                    }
                }
                else if (piecePtr->getColor() == getEnemyColor(playerColor))
                {
                    opponentLocations.push_back(locationVector);
                }
            }
        }

        // find catchable locations...
        std::map<size_t, std::pair<Vector2, Vector2>, std::greater<size_t>> catchableLocations;
        for (const auto& pieceMovablePair : movablePieces)
        {
            for (const auto& movableLocation : pieceMovablePair.second)
            {
                auto piecePtr = board.getCell(pieceMovablePair.first).getPiece();
                auto targetPiece = board.getCell(movableLocation).getPiece();

                if (isPieceTypeOf<King>(piecePtr.get()) && board.isChecked(movableLocation))
                {
                    std::cout << "Warning: Ignore checked king position..." << std::endl;
                    continue;
                }

                if (targetPiece->getColor() == getEnemyColor(playerColor))
                {
                    auto difference = movableLocation - pieceMovablePair.first;
                    catchableLocations.emplace(targetPiece->getPriority(), std::make_pair(pieceMovablePair.first, difference));
                }
            }
        }

        // ...then return if exists.
        if (catchableLocations.size() > 0)
        {
            return catchableLocations.begin()->second;
        }

        std::map<std::pair<Vector2, Vector2>, int> locationScores;
        for (const auto& pieceMovablePair : movablePieces)
        {
            auto piece = board.getCell(pieceMovablePair.first).getPiece();

            for (const auto& movableLocation : pieceMovablePair.second)
            {
                int deltaScore = 0;
                auto difference = movableLocation - pieceMovablePair.first;

                for (const auto& opponentLocation : opponentLocations)
                {
                    double roundedMagnitude = std::round((opponentLocation - movableLocation).magitude());
                    deltaScore += static_cast<int>(roundedMagnitude);
                }

                locationScores[std::make_pair(pieceMovablePair.first, difference)] = isPassivePick ?
                            -(static_cast<int>(piece->getPriority()) / 2) - deltaScore :
                            -(static_cast<int>(piece->getPriority()) / 2) + deltaScore;
            }
        }

        // What is the best location?
        std::pair<Vector2, Vector2> chosenLocation;
        int bestScore = std::numeric_limits<int>::min();

        for (const auto& locationScorePair : locationScores)
        {
            auto piecePtr = board.getCell(locationScorePair.first.first).getPiece().get();

            if (isPieceTypeOf<King>(piecePtr) && board.isChecked(locationScorePair.first.second))
            {
                std::cout << "Warning: Ignore checked king position..." << std::endl;
                continue;
            }

            if ((locationScorePair.second > bestScore) ||
                (locationScorePair.second == bestScore && pickRandomNumber(0, 2) == 0))
            {
                chosenLocation = locationScorePair.first;
                bestScore = locationScorePair.second;
            }
        }

        return chosenLocation;
    }
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;

        throw;
    }
}


#endif // BOARDUTILITY_H
