#pragma once

#include "Action.h"

namespace AI
{

    class ResearchAction : public Action
    {
    public:
        ResearchAction();

        void execute(const std::shared_ptr<GameState> &gameState, const std::shared_ptr<Player> &player);
    };
} // namespace AI
