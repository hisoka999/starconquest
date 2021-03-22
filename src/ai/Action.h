#pragma once

#include <memory>

class GameState;
class Player;

namespace AI
{
    class Action
    {
    public:
        virtual void execute(const std::shared_ptr<GameState> &gameState, const std::shared_ptr<Player> &player) = 0;
    };
} // namespace AI
