/*
 * Building.cpp
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#include "Building.h"

Building::Building(const std::string& name, const std::string& localisedName, int resources)
    : BuildableObject(name, localisedName, resources)
{
    // TODO Auto-generated constructor stub
}

Building::~Building()
{
    // TODO Auto-generated destructor stub
}

bool Building::canBuildObject(int planetResources)
{
    return planetResources > getRessources();
}

void Building::addModifier(ModifierType type, int value)
{
    modifiers[type] = value;
}

int Building::getModifier(ModifierType type)
{
    if (modifiers.count(type) == 0)
        return 0;
    return modifiers[type];
}
