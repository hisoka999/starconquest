/*
 * constants.h
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define PLANET_DISTANCE 50.0f
#define MAX_PLANETS_PER_STAR 4

#define WORLD_SIZE 3200
#define PI 3.14159265f

enum class Difficulty
{
    Easy,
    Normal,
    Hard,
    Impossible
};

enum class WorldSize
{
    Small = 20,
    Medium = 40,
    Big = 80,
    Huge = 120,
};

#endif /* CONSTANTS_H_ */
