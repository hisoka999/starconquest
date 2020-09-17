/*
 * Player.h
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Race.h"
#include <SDL2/SDL.h>
#include <string>

class Player {
public:
    Player(std::string name, Race race, SDL_Color color);
    virtual ~Player();

    std::string getName();
    SDL_Color getColor();
    Race getRace();
    long getMoney();
    void addMoney(long value);

    int getMoneyPerMonth() const;
    void setMoneyPerMonth(int value);

    int getResearchPerMonth() const;
    void setResearchPerMonth(int value);

private:
    std::string name;
    SDL_Color color;
    Race race;
    long money;
    int moneyPerMonth;
    int researchPerMonth;
};

#endif /* PLAYER_H_ */
