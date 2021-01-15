/*
 * Race.cpp
 *
 *  Created on: 01.05.2016
 *      Author: stefan
 */

#include "Race.h"

Race::Race(RaceType Type, std::string Name, std::string HomePlanet)
    : type(Type)
    , name(Name)
    , homePlanet(HomePlanet)
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

std::vector<std::shared_ptr<Research> > Race::getAvailableResearch() const
{
    return availableResearch;
}

void Race::setAvailableResearch(const std::vector<std::shared_ptr<Research> > &value)
{
    availableResearch = value;
}

void Race::setModifier(const AttributeModifierType type,const int value) 
{
    modifiers[type] = value;
}

int Race::getModifier(const AttributeModifierType type) 
{
    return modifiers.at(type);
}


