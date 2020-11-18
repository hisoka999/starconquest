/*
 * Ship.cpp
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#include "Ship.h"

Ship::Ship(const std::string& name, int resources, ShipType type, int costsPerMonth)
    : BuildableObject(name, resources)
    , shipType(type)
    , costsPerMonth(costsPerMonth)
{
    // TODO Auto-generated constructor stub
}

Ship::~Ship()
{
    // TODO Auto-generated destructor stub
}

bool Ship::canBuildObject(int planetResources)
{
    return planetResources > getRessources();
}

void Ship::addAttribute(const Attribute attr, const int value)
{
    attrbuteValues[attr] = value;
}
int Ship::getAttribute(const Attribute attr)
{
    return attrbuteValues.at(attr);
}

ShipType Ship::getShipType() const
{
    return shipType;
}
bool Ship::isDistanceReachable(const float distance)
{
    return getAttribute(Attribute::Drive) * 100 >= distance;
}

int Ship::getCostsPerMonth() const
{
    return costsPerMonth;
}

std::map<Attribute, int> Ship::getAttrbuteValues() const
{
    return attrbuteValues;
}
