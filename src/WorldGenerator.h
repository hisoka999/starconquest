/*
 * WorldGenerator.h
 *
 *  Created on: 25.04.2016
 *      Author: stefan
 */

#ifndef WORLDGENERATOR_H_
#define WORLDGENERATOR_H_

#include "Player.h"
#include "Star.h"
#include <random>
#include <string>
#include <vector>

class WorldGenerator {
public:
    WorldGenerator();
    virtual ~WorldGenerator();

    std::vector<std::shared_ptr<Star>> generateStarsystem(int systemSize,
        std::vector<std::shared_ptr<Player>> players);

private:
    void loadStarnames();
    std::string generateName();
    std::vector<std::string> starNames;
    std::mt19937 generator;
};

#endif /* WORLDGENERATOR_H_ */
