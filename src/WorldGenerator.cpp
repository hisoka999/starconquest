/*
 * WorldGenerator.cpp
 *
 *  Created on: 25.04.2016
 *      Author: stefan
 */

#include "WorldGenerator.h"
#include "constants.h"
#include <chrono>
#include <ctime>
#include <engine/utils/exceptions.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

WorldGenerator::WorldGenerator()
    : generator(std::chrono::system_clock::now().time_since_epoch().count())

{
    loadStarnames();
}

void WorldGenerator::loadStarnames()
{
    std::fstream file;
    std::string filename = "data/starlist";
    std::string line;
    file.open(filename.c_str(), std::ios::in);
    if (!file.is_open()) {
        throw IOException(filename, "file does not exists");
    }
    while (getline(file, line)) {

        starNames.push_back(line);
    }
    //while(!s.empty());
    file.close();
}

std::string WorldGenerator::generateName()
{
    std::uniform_int_distribution<size_t> distribution(0, starNames.size());
    size_t nameId = distribution(generator);
    return starNames[nameId];
}

std::vector<std::shared_ptr<Star>> WorldGenerator::generateStarsystem(int systemSize,
    std::vector<std::shared_ptr<Player>> players)
{
    std::vector<std::shared_ptr<Star>> stars;
    //player star
    std::shared_ptr<Player> currentPlayer = nullptr;
    auto playerList = players;
    int min_distance = (MAX_PLANETS_PER_STAR + 1) * PLANET_DISTANCE;
    std::uniform_int_distribution<int> positionGen(min_distance,
        WORLD_SIZE - min_distance);
    std::uniform_int_distribution<unsigned int> planetGen(0, 4);
    std::uniform_int_distribution<unsigned int> planetSizeGen(5, 10);
    std::uniform_int_distribution<unsigned int> planetTypeGen(0, MAX_PLANETS_PER_STAR);
    std::uniform_int_distribution<unsigned int> angleGen(0, 380);

    std::clock_t c_start = std::clock();
    long seed = std::chrono::system_clock::now().time_since_epoch().count();

    auto t_start = std::chrono::high_resolution_clock::now();

    for (int starId = 0; starId < systemSize; ++starId) {
        std::string starName;
        StarType starType = StarType::Yellow;
        currentPlayer = nullptr;
        unsigned int planetNum = planetGen(generator);

        if (playerList.size() > 0) {
            currentPlayer = playerList.back();
            playerList.pop_back();

            starName = currentPlayer->getRace().getHomePlanet();
            while (planetNum == 0)
                planetNum = planetGen(generator);
        }
        //lookup star name
        if (starName.empty()) {
            starName = generateName();

            if (starName.empty()) {
                throw std::runtime_error("star name is empty");
            }
        }
        bool goodPosition = false;

        // find a good position
        utils::Vector2 position(0, 0);
        while (!goodPosition) {
            position = utils::Vector2(positionGen(generator),
                positionGen(generator));
            goodPosition = true;
            unsigned int dd = 0;
            for (auto& tmpStar : stars) {

                int distance = position.distance(tmpStar->getPosition());
                if (distance < min_distance * 2) {
                    goodPosition = false;
                    break;
                } else if (distance > min_distance * 4) {
                    dd++;
                }
            }
            if (dd == stars.size() && dd != 0)
                goodPosition = false;
        }
        std::shared_ptr<Star> star = std::make_shared<Star>(starName, position, nullptr, starType);

        for (unsigned int planetId = 0; planetId < planetNum; ++planetId) {
            unsigned int planetSize = planetSizeGen(generator);

            PlanetType type = static_cast<PlanetType>(planetTypeGen(generator));
            if (currentPlayer != nullptr && planetId == 0)
                type = PlanetType::Terran;
            std::shared_ptr<Planet> planet = std::make_shared<Planet>(starName + " " + std::to_string(planetId + 1), type, planetSize, angleGen(generator));
            if (currentPlayer != nullptr)
                planet->colonize(currentPlayer);
            planet->generateSurface(seed);
            star->addPlanet(planet);
            currentPlayer = nullptr;
        }
        stars.push_back(star);
    }
    //TODO add connection between stars

    std::clock_t c_end = std::clock();

    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
              << "Wall clock time passed: "
              << std::chrono::duration<double, std::milli>(t_end - t_start).count()
              << " ms\n";

    return stars;
}

WorldGenerator::~WorldGenerator()
{
}
