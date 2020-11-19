#ifndef BUILDINGSERVICE_H
#define BUILDINGSERVICE_H

#include "../Building.h"
#include "jsonservice.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

class BuildingService : public services::JSONService<Building> {
public:
    static BuildingService& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }

protected:
    virtual std::shared_ptr<Building> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object>& object);

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
};

#endif // BUILDINGSERVICE_H
