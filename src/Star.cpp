/*
 * Star.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "Star.h"
#include "constants.h"

Star::Star(const std::string &pName, const utils::Vector2 &pPosition,
           graphics::Texture *pTexture, StarType pType)
    : name(pName), position(pPosition), type(pType)
{
}

void Star::addPlanet(std::shared_ptr<Planet> planet)
{
    planets.push_back(planet);
}
void Star::addRelation(Star &star)
{
    relations.push_back(star);
}

std::string Star::getName()
{
    return name;
}
const std::vector<std::shared_ptr<Planet>> &Star::getPlanets() const
{
    return planets;
}

utils::Vector2 Star::getPosition() const
{
    return position;
}

void Star::setPosition(const utils::Vector2 &pPosition)
{
    position = pPosition;
}

std::shared_ptr<Player> Star::getPlayer()
{
    for (auto &planet : planets)
    {
        if (planet->getPlayer() != nullptr)
            return planet->getPlayer();
    }
    return nullptr;
}
graphics::Rect Star::planetDisplayRect(const std::shared_ptr<Planet> &planet) const
{
    graphics::Rect planetRect;
    int i = 0;
    auto pos = std::find(planets.begin(), planets.end(), planet);
    i = pos - planets.begin() + 1;
    float x = this->getPosition().getX();
    float y = this->getPosition().getY();
    int planetX = std::cos(planet->getAngle() * PI / 180.0) * (i * PLANET_DISTANCE);
    int planetY = std::sin(planet->getAngle() * PI / 180.0) * (i * PLANET_DISTANCE);
    int tmpx = (planet->getSize() * 3 * std::cos(135 * PI / 180.0f)) / 2;
    int tmpy = (planet->getSize() * 3 * std::sin(135 * PI / 180.0f)) / 2;
    planetRect.x = x + planetX + tmpx;

    planetRect.y = y + (planetY + tmpy) * -1;
    planetRect.width = planet->getSize() * 3;
    planetRect.height = planet->getSize() * 3;
    return planetRect;
}
Star::~Star()
{
    // TODO Auto-generated destructor stub
}

StarType Star::getType()
{
    return type;
}
