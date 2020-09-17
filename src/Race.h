/*
 * Race.h
 *
 *  Created on: 01.05.2016
 *      Author: stefan
 */

#ifndef RACE_H_
#define RACE_H_

#include <string>
#include <memory>
#include <vector>
#include "Building.h"

enum class RaceType
{
	Human, Psilons,
};

class Race
{
public:
	Race(RaceType Type, std::string Name, std::string HomePlanet);
	virtual ~Race();

	RaceType getType();
	std::string getName();
	std::string getHomePlanet();

	std::vector<std::shared_ptr<Building> > getAvailableBuildings() const {
		return availableBuildings;
	}

	void setAvailableBuildings(
			const std::vector<std::shared_ptr<Building> >& availableBuildings) {
		this->availableBuildings = availableBuildings;
	}

private:
	RaceType type;
	std::string name;
	std::string homePlanet;
	std::vector<std::shared_ptr<Building>> availableBuildings;
};

#endif /* RACE_H_ */
