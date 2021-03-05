#include "raceservice.h"
#include <engine/graphics/TextureManager.h>

//RaceService *RaceService::instance = nullptr;
std::once_flag RaceService::onceFlag;

RaceService::RaceService()
{
}

std::shared_ptr<Race> RaceService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
{
    RaceType type = RaceType::Human;
    auto name = object->getStringValue("name");
    auto home = object->getObjectValue("home");
    auto homePlanet = home->getStringValue("name");
    std::shared_ptr<Race> race = std::make_shared<Race>(type, name, homePlanet);
    auto faceTexturePath = object->getStringValue("face_image");
    race->setFaceTexture(graphics::TextureManager::Instance().loadTexture(faceTexturePath));

    return race;
}
