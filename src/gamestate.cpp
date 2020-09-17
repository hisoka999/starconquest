#include "gamestate.h"

GameState::GameState(std::vector<std::shared_ptr<Star>> stars,
    std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Player> humanPlayer)
    : stars(stars)
    , players(players)
    , humanPlayer(humanPlayer)
{
}

std::vector<std::shared_ptr<Star>> GameState::getStars() const
{
    return stars;
}

std::vector<std::shared_ptr<Player>> GameState::getPlayers() const
{
    return players;
}

std::shared_ptr<Player> GameState::getHumanPlayer() const
{
    return humanPlayer;
}
std::vector<std::shared_ptr<Star>> GameState::findStarsForPlayer(const std::shared_ptr<Player>& player) const
{
    std::vector<std::shared_ptr<Star>> playerstars;
    for (const auto& star : this->stars) {
        if (star->getPlayer() == player)
            playerstars.push_back(star);
    }
    return playerstars;
}
void GameState::updatePlayerState(const std::shared_ptr<Player>& player)
{
    auto playerStars = findStarsForPlayer(player);
    int moneyPerTurn = 0;
    int researchPerMonth = 0;

    for (const auto& star : stars) {
        for (const auto& planet : star->getPlanets()) {
            if (planet->getPlayer() == player) {
                moneyPerTurn += planet->calculateIncome();
                planet->updatePopulation();
                researchPerMonth += planet->caluclateResearch();
                planet->updateBuildQueue();
            }
        }
    }
    player->setMoneyPerMonth(moneyPerTurn);
    player->addMoney(moneyPerTurn);
    player->setResearchPerMonth(researchPerMonth);
}

void GameState::addFleet(const std::shared_ptr<Fleet>& fleet)
{
    fleets.push_back(fleet);
}

std::vector<std::shared_ptr<Fleet>> GameState::getFleets() const
{
    return fleets;
}
