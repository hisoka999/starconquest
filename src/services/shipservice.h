#ifndef SHIPSERVICE_H
#define SHIPSERVICE_H

#include "../Ship.h"
#include <engine/utils/json/parser.h>
#include <memory>
#include <mutex>
#include <vector>

class ShipService {
public:
    static ShipService& Instance()
    {
        std::call_once(onceFlag, [] {
            initSingleton();
        });

        return *(instance);
    }
    void loadShips(const std::string& fileName);

    std::vector<std::shared_ptr<Ship>> getShips() const;

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

    std::shared_ptr<Ship> convertJsonObject2Ship(const std::shared_ptr<utils::JSON::Object>& object);

    utils::JSON::Parser parser;

    std::vector<std::shared_ptr<Ship>> ships;
};

#endif // SHIPSERVICE_H
