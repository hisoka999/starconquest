#include "researchservice.h"
#include <engine/utils/localisation.h>
#include <fstream>
#include <magic_enum.hpp>

ResearchService* ResearchService::instance = nullptr;
std::once_flag ResearchService::onceFlag;

ResearchService::ResearchService()
{
}
void ResearchService::loadResearch(std::string fileName)
{
    loadData(fileName);

    auto list = getData();

    for (auto& research : list) {

        auto names = research->getRequirementNames();
        for (auto name : names) {
            for (auto& sub : list) {
                if (sub->getName() == name) {
                    research->addRequirement(sub);
                    break;
                }
            }
        }
    }
}

std::shared_ptr<Research> ResearchService::convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object>& object)
{

    std::string lang = Localisation::Instance().getLanguage();
    if (lang == "en")
        lang = "";
    else {
        lang = "_" + lang;
    }
    std::string name = object->getStringValue("name");
    std::string icon = object->getStringValue("icon");
    std::string localisedName = object->getStringValue("name" + lang);

    std::string description = object->getStringValue("description");

    int costsPerMonth = object->getIntValue("costs");

    std::shared_ptr<Research> research = std::make_shared<Research>(name, localisedName, description, icon, costsPerMonth);

    auto enabledObjs = object->getArray("enables");
    auto requirements = object->getArray("requires");
    std::vector<std::string> enabledObjectNames;
    std::vector<std::string> requirementNames;
    for (auto obj : enabledObjs) {
        auto objName = std::get<std::string>(obj);
        enabledObjectNames.push_back(objName);
    }

    for (auto obj : requirements) {
        auto objName = std::get<std::string>(obj);
        requirementNames.push_back(objName);
    }
    research->setEnabledObjects(enabledObjectNames);
    research->setRequirementNames(requirementNames);

    return research;
}
