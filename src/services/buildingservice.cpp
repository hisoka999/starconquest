#include "buildingservice.h"
#include <engine/utils/exceptions.h>
#include <fstream>
#include <magic_enum.hpp>
#include <sstream>

BuildingService* BuildingService::instance = nullptr;
std::once_flag BuildingService::onceFlag;

BuildingService::BuildingService()
{

}


std::shared_ptr<Building> BuildingService::convertJsonObject2Building(const std::shared_ptr<utils::JSON::Object>& object)
{
    auto name = object->getStringValue("name");
    int resources = object->getIntValue("resources");
    std::shared_ptr<Building> building = std::make_shared<Building>(name,resources);
    building->loadTexture(object->getStringValue("texture"));
    auto props = object->getObjectValue("props");
    for(auto attrName : props->getAttributes())
    {
        building->addModifier(magic_enum::enum_cast<ModifierType>(attrName).value(),props->getIntValue(attrName));
    }
    return building;
}

void BuildingService::loadBuildings(const std::string& fileName)
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

    //std::cout << buffer << std::endl;

    auto objects = parser.parseArray(buffer);

    for (auto& object : objects) {
        buildings.push_back(convertJsonObject2Building(std::get<std::shared_ptr<utils::JSON::Object>>(object)));
    }
}

std::vector<std::shared_ptr<Building>> BuildingService::getBuildings() const
{
    return buildings;
}
