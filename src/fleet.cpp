#include "fleet.h"
#include <algorithm>

Fleet::Fleet(const std::string& name)
    : name(name)
    , owner(nullptr)
    , position(0, 0)
    , targetPosition(0, 0)
{
}

void Fleet::addShip(const std::shared_ptr<Ship>& ship)
{
    ships.push_back(ship);
}

std::vector<std::shared_ptr<Ship>> Fleet::getShips() const
{
    return ships;
}

std::string Fleet::getName() const
{
    return name;
}

Player* Fleet::getOwner() const
{
    return owner;
}

void Fleet::setOwner(Player* value)
{
    owner = value;
}

utils::Vector2 Fleet::getPosition() const
{
    return position;
}

void Fleet::setPosition(const utils::Vector2& value)
{
    position = value;
}

utils::Vector2 Fleet::getTargetPosition() const
{
    return targetPosition;
}

void Fleet::setTargetPosition(const utils::Vector2& value)
{
    targetPosition = value;
}

utils::Vector2 Fleet::getStartPosition() const
{
    return startPosition;
}

void Fleet::setStartPosition(const utils::Vector2& value)
{
    startPosition = value;
}
std::shared_ptr<Ship> Fleet::getFirstShip() const
{
    return ships.front();
}
void Fleet::destroyShip(const std::shared_ptr<Ship>& ship)
{
    auto pos = std::find(ships.begin(), ships.end(), ship);
    if (pos != std::end(ships)) {
        ships.erase(pos);
    }
}

int Fleet::calculateCosts()
{
    int costs = 0;
    for (auto& ship : ships) {
        costs += ship->getCostsPerMonth();
    }
    return costs;
}
