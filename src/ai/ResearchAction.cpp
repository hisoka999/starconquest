#include "ResearchAction.h"
#include "../Player.h"
#include <iostream>
namespace AI
{
    ResearchAction::ResearchAction()
        : Action()
    {
    }

    void ResearchAction::execute(const std::shared_ptr<GameState> &gameState, const std::shared_ptr<Player> &player)
    {

        auto queue = player->getResearchQueue();

        auto &availableResearch = player->getRace().getAvailableResearch();
        if (queue.size() == 0)
        {
            //at first just research the first available item
            if (availableResearch.size() > 0)
            {
                auto &research = availableResearch.front();
                queue.push_back(research);
            }
        }
    }
}