#include "shipservice.h"
#include <engine/utils/exceptions.h>
#include <engine/utils/localisation.h>
#include <fstream>
#include <magic_enum.hpp>
#include <sstream>

ShipService* ShipService::instance = nullptr;
std::once_flag ShipService::onceFlag;

ShipService::ShipService()
{
}

std::shared_ptr<Ship> ShipService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object>& object)
{
    std::string lang = Localisation::Instance().getLanguage();
    if (lang == "en")
        lang = "";
    else {
        lang = "_" + lang;
    }
    std::string name = object->getStringValue("name");
    std::string localisedName = object->getStringValue("name" + lang);
    int res = object->getIntValue("ressources");
    ShipType type = magic_enum::enum_cast<ShipType>(object->getStringValue("type")).value();
    ; //TODO
    int costsPerMonth = object->getIntValue("costs");

    std::shared_ptr<Ship> ship = std::make_shared<Ship>(name, localisedName, res, type, costsPerMonth);
    ship->loadTexture(object->getStringValue("texture"));

    auto props = object->getObjectValue("properties");

    for (auto& prop : props->getAttributes()) {
        Attribute attrEnum = magic_enum::enum_cast<Attribute>(prop).value();
        ship->addAttribute(attrEnum, props->getIntValue(prop));
    }
    return ship;
}
