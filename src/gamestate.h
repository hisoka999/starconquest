#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "Star.h"
#include "fleet.h"

class GameState {
public:
    GameState(std::vector<std::shared_ptr<Star>> stars,
        std::vector<std::shared_ptr<Player>> players, std::shared_ptr<Player> humanPlayer);

    std::vector<std::shared_ptr<Star>> getStars() const;

    std::vector<std::shared_ptr<Player>> getPlayers() const;

    std::shared_ptr<Player> getHumanPlayer() const;

    std::vector<std::shared_ptr<Star>> findStarsForPlayer(const std::shared_ptr<Player>& player) const;

    void updatePlayerState(const std::shared_ptr<Player>& player);
    void addFleet(const std::shared_ptr<Fleet>& fleet);

    std::vector<std::shared_ptr<Fleet>> getFleets() const;

private:
    std::vector<std::shared_ptr<Star>> stars;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Player> humanPlayer;
    std::vector<std::shared_ptr<Fleet>> fleets;
};

#endif // GAMESTATE_H
