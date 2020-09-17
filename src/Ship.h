/*
 * Ship.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef SHIP_H_
#define SHIP_H_

#include "BuildableObject.h"
#include "Player.h"

enum class Attribute {
    Shield,
    Bombs,
    Hull,
    Rockets,

};

enum class ShipType {
    ColonyShip,
    Transport,
    Scout,
    NormalShip,
};

class Ship : public BuildableObject {
public:
    Ship(const std::string& name, int resources, ShipType type);
    virtual ~Ship();
    virtual bool canBuildObject(int planetResources);

    void addAttribute(const Attribute attr, const int value);
    int getAttribute(const Attribute attr);

    ShipType getShipType() const;

private:
    ShipType shipType;
    std::map<Attribute, int> attrbuteValues;
};

#endif /* SHIP_H_ */
