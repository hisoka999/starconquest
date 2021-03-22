#pragma once
#include "Action.h"
#include <map>
#include "../Planet.h"
class BuildableObject;

namespace AI
{
    class BuildAction : public Action
    {
    public:
        BuildAction();
        void execute(const std::shared_ptr<GameState> &gameState, const std::shared_ptr<Player> &player);

    private:
        void buildBuilding(const std::shared_ptr<BuildableObject> &building, const std::shared_ptr<Planet> &planet);
        std::map<std::string, FieldType> optimalFieldType;
    };
} // namespace AI
