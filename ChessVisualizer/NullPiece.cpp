#include "NullPiece.h"
#include <memory>

std::shared_ptr<NullPiece> sharedNullPiece = std::make_shared<NullPiece>();

bool NullPiece::isStrategyAcceptable([[maybe_unused]] const std::unique_ptr<MovementStrategy>& strategy) const noexcept
{
    return false;
}
