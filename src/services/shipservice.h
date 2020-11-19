#ifndef SHIPSERVICE_H
#define SHIPSERVICE_H

#include "../Ship.h"
#include "jsonservice.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

class ShipService : public services::JSONService<Ship> {
public:
    static ShipService& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }

protected:
    std::shared_ptr<Ship> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object>& object);

private:
    ShipService();
    ~ShipService() = default;
    ShipService(const ShipService&) = delete;
    ShipService& operator=(const ShipService&) = delete;

    static ShipService* instance;
    static std::once_flag onceFlag;

    static void initSingleton()
    {
        instance = new ShipService();
    }
};

#endif // SHIPSERVICE_H
