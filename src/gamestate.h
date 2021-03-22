#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Star.h"
#include "fleet.h"

class GameState
{
public:
    GameState(std::vector<std::shared_ptr<Star>> stars,
              std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Player> humanPlayer, uint32_t systemSize);
    virtual ~GameState();

    const std::vector<std::shared_ptr<Star>> &getStars() const;

    const std::vector<std::shared_ptr<Player>> &getPlayers() const;

    const std::shared_ptr<Player> &getHumanPlayer() const;

    const uint32_t &getSystemSize() const;

    std::vector<std::shared_ptr<Star>> findStarsForPlayer(const std::shared_ptr<Player> &player) const;

    void updatePlayerState(const std::shared_ptr<Player> &player);
    void addFleet(const std::shared_ptr<Fleet> &fleet);

    const std::vector<std::shared_ptr<Fleet>> &getFleets() const;
    void removeFleet(const std::shared_ptr<Fleet> &fleet);

    void updateFleetPosition(float deltaTime);

    std::vector<std::shared_ptr<Fleet>> findFleetsInPlanetDistance(const std::shared_ptr<Star> &star, const std::shared_ptr<Planet> &planet) const;

private:
    std::vector<std::shared_ptr<Star>> stars;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> humanPlayer;
    std::vector<std::shared_ptr<Fleet>> fleets;
    uint32_t systemSize;
};

#endif // GAMESTATE_H
