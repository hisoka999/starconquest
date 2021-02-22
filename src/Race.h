/*
 * Race.h
 *
 *  Created on: 01.05.2016
 *      Author: stefan
 */

#ifndef RACE_H_
#define RACE_H_

#include "Building.h"
#include "research.h"
#include <memory>
#include <string>
#include <vector>

enum class RaceType
{
    Human,
    Psilons,
};

enum class AttributeModifierType
{
    Income,
    Research,
    Production,
    Food,
    NeedsFood,
};

struct AttributeModifier
{
    AttributeModifierType type;
    int value;
};

class Race
{
public:
    Race(RaceType Type, std::string Name, std::string HomePlanet);
    virtual ~Race();

    RaceType getType();
    std::string getName();
    std::string getHomePlanet();

    std::vector<std::shared_ptr<Building>> getAvailableBuildings() const
    {
        return availableBuildings;
    }

    void setAvailableBuildings(
        const std::vector<std::shared_ptr<Building>> &availableBuildings)
    {
        this->availableBuildings = availableBuildings;
    }

    const std::vector<std::shared_ptr<Research>> &getAvailableResearch() const;
    void setAvailableResearch(const std::vector<std::shared_ptr<Research>> &value);

    void setModifier(const AttributeModifierType type, const int value);

    int getModifier(const AttributeModifierType type);

    void setFaceTexture(const std::shared_ptr<graphics::Texture> &texture);
    const std::shared_ptr<graphics::Texture> &getFaceTexture() const;

private:
    RaceType type;
    std::string name;
    std::string homePlanet;
    std::vector<std::shared_ptr<Building>> availableBuildings;
    std::vector<std::shared_ptr<Research>> availableResearch;

    std::map<AttributeModifierType, int> modifiers;
    std::shared_ptr<graphics::Texture> faceTexture;
};

#endif /* RACE_H_ */
