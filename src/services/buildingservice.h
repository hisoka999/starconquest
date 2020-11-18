#ifndef BUILDINGSERVICE_H
#define BUILDINGSERVICE_H

#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>
#include "../Building.h"

class BuildingService
{
public:

    static BuildingService& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }
    void loadBuildings(const std::string& fileName);

    std::vector<std::shared_ptr<Building>> getBuildings() const;

private:
    BuildingService();
    ~BuildingService() = default;
    BuildingService(const BuildingService&) = delete;
    BuildingService& operator=(const BuildingService&) = delete;

    static BuildingService* instance;
    static std::once_flag onceFlag;

    static void initSingleton()
    {
        instance = new BuildingService();
    }

    std::shared_ptr<Building> convertJsonObject2Building(const std::shared_ptr<utils::JSON::Object>& object);

    utils::JSON::Parser parser;

    std::vector<std::shared_ptr<Building>> buildings;
};

#endif // BUILDINGSERVICE_H
