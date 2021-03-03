/*
 * Star.h
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#ifndef STAR_H_
#define STAR_H_

#include "Planet.h"
#include <engine/ui/ImageButton.h>
#include <memory>
#include <vector>

enum class StarType
{
    Yellow,
    Red,
};

class Star
{
public:
    Star(const std::string &pName, const utils::Vector2 &pPosition,
         graphics::Texture *pTexture, StarType pType);
    virtual ~Star();

    void addPlanet(std::shared_ptr<Planet> planet);
    void addRelation(Star &star);

    std::string getName();
    const std::vector<std::shared_ptr<Planet>> &getPlanets() const;

    utils::Vector2 getPosition() const;
    void setPosition(const utils::Vector2 &pPosition);

    std::shared_ptr<Player> getPlayer();

    StarType getType();

    graphics::Rect planetDisplayRect(const std::shared_ptr<Planet> &planet) const;

private:
    std::string name;
    std::vector<std::shared_ptr<Planet>> planets;

    std::vector<Star> relations;

    utils::Vector2 position;

    StarType type;
};

#endif /* STAR_H_ */
