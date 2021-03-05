#include "buildingservice.h"
#include <engine/utils/exceptions.h>
#include <engine/utils/localisation.h>
#include <fstream>
#include <magic_enum.hpp>
#include <sstream>

BuildingService::BuildingService()
{
}

std::shared_ptr<Building> BuildingService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
{
    std::string lang = Localisation::Instance().getLanguage();
    if (lang == "en")
        lang = "";
    else
    {
        lang = "_" + lang;
    }
    auto name = object->getStringValue("name");
    auto localisedName = object->getStringValue("name" + lang);
    int resources = object->getIntValue("resources");
    std::shared_ptr<Building> building = std::make_shared<Building>(name, localisedName, resources);
    building->loadTexture(object->getStringValue("texture"));
    auto props = object->getObjectValue("props");
    for (auto attrName : props->getAttributes())
    {
        building->addModifier(magic_enum::enum_cast<ModifierType>(attrName).value(), props->getIntValue(attrName));
    }
    return building;
}
