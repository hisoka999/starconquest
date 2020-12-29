/*
 * Player.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "Player.h"
#include <algorithm>

Player::Player(std::string name, Race race, SDL_Color color)
    : name(name)
    , color(color)
    , race(race)
    , money(200)
    , moneyPerMonth(0)
    , researchPerMonth(0)
{
}

Player::~Player()
{
}

std::string Player::getName()
{
    return name;
}
SDL_Color Player::getColor()
{
    return color;
}
Race& Player::getRace()
{
    return race;
}
long Player::getMoney()
{
    return money;
}
void Player::addMoney(long value)
{
    money += value;
}

int Player::getMoneyPerMonth() const
{
    return moneyPerMonth;
}

void Player::setMoneyPerMonth(int value)
{
    moneyPerMonth = value;
}

int Player::getResearchPerMonth() const
{
    return researchPerMonth;
}

void Player::setResearchPerMonth(int value)
{
    researchPerMonth = value;
}

std::vector<std::shared_ptr<Research>> Player::getResearchQueue() const
{
    return researchQueue;
}

void Player::addResearchToQueue(const std::shared_ptr<Research>& research)
{
    auto found = std::find(researchQueue.begin(), researchQueue.end(), research);
    if (found == std::end(researchQueue)) {
        researchQueue.push_back(research);
    }
}
void Player::research()
{
    if (researchQueue.size() == 0)
        return;

    auto& currentResearch = researchQueue.front();

    currentResearch->reduceCosts(getResearchPerMonth());
    if (currentResearch->getResearched()) {
        researchQueue.erase(researchQueue.begin());
    }
}
