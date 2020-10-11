#include "shipservice.h"
#include <engine/utils/exceptions.h>
#include <fstream>
#include <sstream>

ShipService* ShipService::instance = nullptr;
std::once_flag ShipService::onceFlag;

ShipService::ShipService()
{
}
void ShipService::loadShips(const std::string& fileName)
{
    std::ifstream file;
    std::istringstream is;
    std::string s;
    std::string group;
    //  std::cout << filename << std::endl;

    file.open(fileName.c_str(), std::ios::in);
    if (!file.is_open()) {
        throw IOException(fileName, "file does not exists");
    }
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);

    auto objects = parser.parseArray(buffer);

    for (auto& object : objects) {
        ships.push_back(convertJsonObject2Ship(std::get<std::shared_ptr<utils::JSON::Object>>(object)));
    }
}
std::shared_ptr<Ship> ShipService::convertJsonObject2Ship(const std::shared_ptr<utils::JSON::Object>& object)
{
    std::string name = object->getStringValue("name");
    int res = object->getIntValue("ressources");
    ShipType type = ShipType::Scout; //TODO
    int costsPerMonth = object->getIntValue("costs");

    std::shared_ptr<Ship> ship = std::make_shared<Ship>(name, res, type, costsPerMonth);

    for (auto& prop : object->getArray("properties")) {
    }
    return ship;
}
