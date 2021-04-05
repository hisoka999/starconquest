#include "BuildAction.h"
#include "../gamestate.h"
#include <memory>

namespace AI
{
    BuildAction::BuildAction() : Action()
    {
        optimalFieldType.insert(std::pair<std::string, FieldType>("Farm", FieldType::Grass));
        optimalFieldType.insert(std::pair<std::string, FieldType>("Factory", FieldType::Forrest));
        optimalFieldType.insert(std::pair<std::string, FieldType>("Shipyard", FieldType::Grass));
        optimalFieldType.insert(std::pair<std::string, FieldType>("Mine", FieldType::Mounten));
        optimalFieldType.insert(std::pair<std::string, FieldType>("Research Lab", FieldType::Forrest));
    }

    void BuildAction::execute(const std::shared_ptr<GameState> &gameState, const std::shared_ptr<Player> &player)
    {

        const auto &stars = gameState->findStarsForPlayer(player);

        for (auto &star : stars)
        {
            for (auto &planet : star->getPlanets())
            {
                if (planet->getPlayer() == player)
                {
                    if (planet->getQueuedObjects().size() == 0)
                    {
                        std::map<std::string, std::shared_ptr<BuildableObject>> buildings;

                        unsigned maxNum = 0;
                        for (auto &building : planet->getBuildableBuildings(0, 0))
                        {
                            buildings[building->getName()] = building;
                            maxNum = std::max(maxNum, planet->countBuildingOfName(building->getName()));
                        }
                        //step one find buildable space
                        auto buildables = planet->getBuildableBuildings(0, 0);
                        std::sort(buildables.begin(), buildables.end(), [&](const std::shared_ptr<BuildableObject> &b1, const std::shared_ptr<BuildableObject> &b2) {
                            return planet->countBuildingOfName(b1->getName()) < planet->countBuildingOfName(b2->getName());
                        });

                        //do we have the space port researched

                        for (auto &building : buildables)
                        {
                            unsigned buildingCount = planet->countBuildingOfName(building->getName());
                            if (!planet->hasBuildingOfName(building->getName()) || buildingCount <= maxNum)
                            {
                                auto realBuilding = std::dynamic_pointer_cast<Building>(building);

                                if (buildingCount < realBuilding->getLimit())
                                {
                                    std::cout << building->getName() << " limit: " << realBuilding->getLimit() << std::endl;
                                    buildBuilding(building, planet);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void BuildAction::buildBuilding(const std::shared_ptr<BuildableObject> &building, const std::shared_ptr<Planet> &planet)
    {
        for (int row = 0; row < planet->getGridHeight(); ++row)
        {
            for (int col = 0; col < planet->getGridWidth(); ++col)
            {

                if (!planet->hasFieldBuilding(row, col) && planet->getField(row, col) == optimalFieldType[building->getName()])
                {
                    int pos = col + (row * FIELDS_PER_ROW);
                    planet->addBuildingToQueue(pos, building);
                    return;
                }
            }
        }
    }
}