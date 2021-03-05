/*
 * Race.cpp
 *
 *  Created on: 01.05.2016
 *      Author: stefan
 */

#include "Race.h"

Race::Race(RaceType Type, std::string Name, std::string HomePlanet)
    : type(Type), name(Name), homePlanet(HomePlanet)
{
    // TODO Auto-generated constructor stub
}

Race::~Race()
{
    // TODO Auto-generated destructor stub
}

RaceType Race::getType()
{
    return type;
}
std::string Race::getName()
{
    return name;
}
std::string Race::getHomePlanet()
{
    return homePlanet;
}

void Race::setAvailableBuildings(const std::vector<std::shared_ptr<Building>> &availableBuildings)
{
    this->availableBuildings.clear();
    for (auto &building : availableBuildings)
    {
        this->availableBuildings.push_back(std::make_shared<Building>(*building));
    }
}

const std::vector<std::shared_ptr<Research>> &Race::getAvailableResearch() const
{
    return availableResearch;
}

void Race::setAvailableResearch(const std::vector<std::shared_ptr<Research>> &value)
{
    this->availableResearch.clear();
    for (auto &research : value)
    {
        this->availableResearch.push_back(std::make_shared<Research>(*research));
    }

    for (auto &research : availableResearch)
    {

        auto names = research->getRequirementNames();
        for (auto name : names)
        {
            for (auto &sub : availableResearch)
            {
                if (sub->getName() == name)
                {
                    research->addRequirement(sub);
                    break;
                }
            }
        }
    }
}

void Race::setModifier(const AttributeModifierType type, const int value)
{
    modifiers[type] = value;
}

int Race::getModifier(const AttributeModifierType type)
{
    return modifiers.at(type);
}

void Race::setFaceTexture(const std::shared_ptr<graphics::Texture> &texture)
{
    faceTexture = texture;
}

const std::shared_ptr<graphics::Texture> &Race::getFaceTexture() const
{
    return faceTexture;
}
