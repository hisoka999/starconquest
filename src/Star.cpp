/*
 * Star.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "Star.h"

Star::Star(const std::string& pName, const utils::Vector2& pPosition,
    graphics::Texture* pTexture, StarType pType)
    : name(pName)
    , position(pPosition)
    , type(pType)
{
}

void Star::addPlanet(std::shared_ptr<Planet> planet)
{
    planets.push_back(planet);
}
void Star::addRelation(Star& star)
{
    relations.push_back(star);
}

std::string Star::getName()
{
    return name;
}
std::vector<std::shared_ptr<Planet>> Star::getPlanets() const
{
    return planets;
}

utils::Vector2& Star::getPosition()
{
    return position;
}

std::shared_ptr<Player> Star::getPlayer()
{
    for (auto& planet : planets) {
        if (planet->getPlayer() != nullptr)
            return planet->getPlayer();
    }
    return nullptr;
}

Star::~Star()
{
    // TODO Auto-generated destructor stub
}

StarType Star::getType()
{
    return type;
}
