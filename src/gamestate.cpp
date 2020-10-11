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
    //get ship costs
    for (auto& fleet : fleets) {
        if (fleet->getOwner() == player.get()) {
            moneyPerTurn -= fleet->calculateCosts();
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
void GameState::removeFleet(const std::shared_ptr<Fleet>& fleet)
{
    auto pos = std::find(fleets.begin(), fleets.end(), fleet);
    if (pos != std::end(fleets)) {
        fleets.erase(pos);
    }
}
std::vector<std::shared_ptr<Fleet>> GameState::findFleetsInPlanetDistance(const std::shared_ptr<Star>& star, const std::shared_ptr<Planet>& planet) const
{
    std::vector<std::shared_ptr<Fleet>> result;
    //get planet position
    auto planetRect = star->planetDisplayRect(planet);

    for (auto fleet : fleets) {
        graphics::Rect fleetRect;
        auto ship = fleet->getFirstShip();
        auto texture = ship->getTexture();
        fleetRect.x = fleet->getPosition().getX();
        fleetRect.y = fleet->getPosition().getY();
        fleetRect.width = texture->getWidth() * 0.2f;
        fleetRect.height = texture->getHeight() * 0.2f;

        if (fleetRect.intersects(planetRect)) {
            result.push_back(fleet);
        }
    }
    return result;
}

void GameState::updateFleetPosition(float deltaTime)
{
    for (auto& fleet : fleets) {
        if (fleet->getPosition() != fleet->getTargetPosition()) {
            float currentDistance = fleet->getPosition().distance(fleet->getStartPosition());
            float fullDistance = fleet->getTargetPosition().distance(fleet->getStartPosition());
            float percentage = currentDistance / fullDistance;
            percentage += (0.1f * deltaTime / 1000.f);
            if (percentage > 1.f) {
                percentage = 1.0f;
            }
            auto pos = utils::lerp(fleet->getStartPosition(), fleet->getTargetPosition(), percentage);
            fleet->setPosition(pos);
        }
    }
}
