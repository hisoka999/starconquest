#ifndef BUILDINGSERVICE_H
#define BUILDINGSERVICE_H

#include "../Building.h"
#include "jsonservice.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

class BuildingService : public services::JSONService<Building>
{
public:
    static BuildingService &Instance()
    {
        static BuildingService instance;
        return instance;
    }

protected:
    virtual std::shared_ptr<Building> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object);

private:
    BuildingService();
    ~BuildingService() = default;
    BuildingService(const BuildingService &) = delete;
    BuildingService &operator=(const BuildingService &) = delete;
};

#endif // BUILDINGSERVICE_H
