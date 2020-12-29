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

enum class RaceType {
    Human,
    Psilons,
};

class Race {
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
        const std::vector<std::shared_ptr<Building>>& availableBuildings)
    {
        this->availableBuildings = availableBuildings;
    }

    std::vector<std::shared_ptr<Research>> getAvailableResearch() const;
    void setAvailableResearch(const std::vector<std::shared_ptr<Research>>& value);

private:
    RaceType type;
    std::string name;
    std::string homePlanet;
    std::vector<std::shared_ptr<Building>> availableBuildings;
    std::vector<std::shared_ptr<Research>> availableResearch;
};

#endif /* RACE_H_ */
