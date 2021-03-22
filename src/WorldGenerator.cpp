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

WorldGenerator::WorldGenerator(GenerationType generationType, unsigned long seed)
    : generator(seed), generationType(generationType), seed(seed)

{
    loadStarnames();
}

void WorldGenerator::loadStarnames()
{
    std::fstream file;
    std::string filename = "data/starlist";
    std::string line;
    file.open(filename.c_str(), std::ios::in);
    if (!file.is_open())
    {
        throw IOException(filename, "file does not exists");
    }
    while (getline(file, line))
    {

        starNames.push_back(line);
    }
    //while(!s.empty());
    file.close();
}

std::string WorldGenerator::generateName()
{
    std::uniform_int_distribution<size_t> distribution(0, starNames.size() - 1);
    size_t nameId = distribution(generator);
    return starNames[nameId];
}

std::vector<std::shared_ptr<Star>> WorldGenerator::generateStarsystem(int systemSize,
                                                                      int worldSize,
                                                                      std::vector<std::shared_ptr<Player>> players)
{
    std::vector<std::shared_ptr<Star>> stars;
    //player star
    std::shared_ptr<Player> currentPlayer = nullptr;
    auto playerList = players;
    int min_distance = (MAX_PLANETS_PER_STAR + 1) * PLANET_DISTANCE;
    std::uniform_int_distribution<int> positionGen(min_distance,
                                                   worldSize - min_distance);
    std::uniform_int_distribution<unsigned int> planetGen(0, 4);
    std::uniform_int_distribution<unsigned int> planetSizeGen(5, 10);
    std::uniform_int_distribution<unsigned int> planetTypeGen(0, MAX_PLANETS_PER_STAR);
    std::uniform_int_distribution<unsigned int> angleGen(0, 380);

    std::clock_t c_start = std::clock();

    auto t_start = std::chrono::high_resolution_clock::now();

    // generate stars
    for (int starId = 0; starId < systemSize; ++starId)
    {
        std::string starName;
        StarType starType = StarType::Yellow;
        unsigned int planetNum = planetGen(generator);
        currentPlayer = nullptr;
        if (playerList.size() > 0)
        {
            currentPlayer = playerList.back();
            playerList.pop_back();

            starName = currentPlayer->getRace().getHomePlanet();
            while (planetNum == 0)
                planetNum = planetGen(generator);

            //remove starname from starlist
            auto it = std::find(starNames.begin(), starNames.end(), starName);
            starNames.erase(it);
        }
        //lookup star name
        if (starName.empty())
        {
            starName = generateName();

            if (starName.empty())
            {
                throw std::runtime_error("star name is empty");
            }
        }
        utils::Vector2 position(0, 0);
        std::shared_ptr<Star> star = std::make_shared<Star>(starName, position, nullptr, starType);

        for (unsigned int planetId = 0; planetId < planetNum; ++planetId)
        {
            unsigned int planetSize = planetSizeGen(generator);

            PlanetType type = static_cast<PlanetType>(planetTypeGen(generator));
            if (currentPlayer != nullptr && planetId == 0)
                type = PlanetType::Terran;
            std::shared_ptr<Planet> planet = std::make_shared<Planet>(starName + " " + std::to_string(planetId + 1), type, planetSize, angleGen(generator));
            if (currentPlayer != nullptr)
                planet->colonize(currentPlayer, true);
            planet->generateSurface(seed);
            star->addPlanet(planet);
            currentPlayer = nullptr;
        }
        stars.push_back(star);
    }

    std::random_shuffle(stars.begin(), stars.end());

    for (auto &star : stars)
    {
        bool goodPosition = false;
        utils::Vector2 position(0, 0);
        while (!goodPosition)
        {

            switch (generationType)
            {
            case GenerationType::Random:
                position = utils::Vector2(positionGen(generator),
                                          positionGen(generator));
                break;

            case GenerationType::Spiral:
            {
                int maxRadius = worldSize / 2;
                std::uniform_int_distribution<unsigned int> radiusGen(5, maxRadius);

                int radius = radiusGen(generator);
                int angle = angleGen(generator);
                int x = radius * std::cos(angle);
                int y = radius * std::sin(angle);
                position = utils::Vector2(x, y);
                break;
            }
            default:
                throw std::runtime_error("unknown generationtype");
            }

            goodPosition = true;
            unsigned int dd = 0;
            for (auto &tmpStar : stars)
            {

                int distance = position.distance(tmpStar->getPosition());
                if (distance < min_distance * 2)
                {
                    goodPosition = false;
                    break;
                }
                else if (distance > min_distance * 4)
                {
                    dd++;
                }
            }
            if (dd == stars.size() && dd != 0)
                goodPosition = false;
        }
        star->setPosition(position);
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
