#ifndef FLEET_H
#define FLEET_H

#include "Ship.h"
#include <memory>
#include <vector>

class Fleet {
public:
    Fleet(const std::string& name);

    void addShip(const std::shared_ptr<Ship>& ship);

    std::vector<std::shared_ptr<Ship>> getShips() const;

    std::shared_ptr<Ship> getFirstShip() const;

    std::string getName() const;
    Player* getOwner() const;
    void setOwner(Player* value);

    utils::Vector2 getPosition() const;
    void setPosition(const utils::Vector2& value);

    utils::Vector2 getTargetPosition() const;
    void setTargetPosition(const utils::Vector2& value);

    utils::Vector2 getStartPosition() const;
    void setStartPosition(const utils::Vector2& value);

private:
    std::string name;
    Player* owner;
    utils::Vector2 position;
    utils::Vector2 startPosition;
    utils::Vector2 targetPosition;
    std::vector<std::shared_ptr<Ship>> ships;
};

#endif // FLEET_H
