/*
 * StarMapScene.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "StarMapScene.h"
#include "../constants.h"
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/utils/os.h>
#define PI 3.14159265

namespace scenes {

StarMapScene::~StarMapScene()
{
}

StarMapScene::StarMapScene(core::Renderer* pRenderer, std::vector<std::shared_ptr<Star>> pStars,
    std::shared_ptr<Player> player)
    : core::Scene(pRenderer)
    , selectedFleet(nullptr)
    , mouseSpeedX(100)
    , mouseSpeedY(100)

{

    std::vector<std::shared_ptr<Player>> players;

    players.push_back(player);
    gameState = std::make_shared<GameState>(pStars, players, player);
    auto colonyShip = std::make_shared<Ship>("ColonyShip", 500, ShipType::ColonyShip);

    auto playerStars = gameState->findStarsForPlayer(gameState->getHumanPlayer());
    auto playerPlanet = playerStars[0]->getPlanets()[0];

    colonyShip->loadTexture("images/ships/colonyship.png");

    colonyShip->addAttribute(Attribute::Hull, 10);
    colonyShip->addAttribute(Attribute::Drive, 5);
    auto fleet = std::make_shared<Fleet>("Fleet 1");
    fleet->setOwner(player.get());
    fleet->addShip(colonyShip);
    fleet->setPosition(playerStars[0]->getPosition());
    fleet->setTargetPosition(playerStars[0]->getPosition());
    gameState->addFleet(fleet);

    timeThread = std::make_unique<TimeThread>(gameState);
    starText.openFont("fonts/Audiowide-Regular.ttf", 12);
    glyphText.openFont("fonts/fa-solid-900.ttf", 20);

    background.loadTexture(renderer, "images/star_background.png");

    uiTexture = graphics::TextureManager::Instance().loadTexture("images/ArkanaLook.png");
    uiText.openFont("fonts/Audiowide-Regular.ttf", 12);

    starTextures[StarType::Yellow] = new graphics::Texture();
    starTextures[StarType::Yellow]->loadTexture(renderer,
        "images/yellow_sun.png");

    planetTextures[PlanetType::Barren] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_23.png");

    planetTextures[PlanetType::Gaia] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_31.png");

    planetTextures[PlanetType::Radiated] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_29.png");

    planetTextures[PlanetType::Swamp] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_23.png");

    planetTextures[PlanetType::Terran] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_27.png");

    planetTextures[PlanetType::Wasteland] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_30.png");
    viewPort = renderer->getViewPort();
    viewPort.width = WORLD_SIZE;
    viewPort.height = WORLD_SIZE;
    defaultViewPort = renderer->getViewPort();

    planetWindow.setPos(100, 100);
    planetWindow.setVisible(false);
    container = std::make_shared<UI::Container>();

    playButton = std::make_shared<UI::Button>();
    playButton->setFont("fonts/fa-solid-900.ttf", 20);
    playButton->setLabel("\uf04b");
    playButton->setPos(350, 0);
    playButton->setBorderless(true);
    playButton->connect(UI::Button::buttonClickCallback(), [&] {
        timeThread->start();
        timeThread->setSpeed(400);
    });
    pauseButton = std::make_shared<UI::Button>();
    pauseButton->setFont("fonts/fa-solid-900.ttf", 20);
    pauseButton->setLabel("\uf04c");
    pauseButton->setPos(380, 0);
    pauseButton->setBorderless(true);
    pauseButton->connect(UI::Button::buttonClickCallback(), [&] {
        timeThread->pause();
    });

    doubleSpeed = std::make_shared<UI::Button>();
    doubleSpeed->setFont("fonts/fa-solid-900.ttf", 20);
    doubleSpeed->setLabel("\uf04e");
    doubleSpeed->setPos(410, 0);
    doubleSpeed->setBorderless(true);
    doubleSpeed->connect(UI::Button::buttonClickCallback(), [&] {
        timeThread->setSpeed(200);
    });

    container->addObject(playButton);
    container->addObject(pauseButton);
    container->addObject(doubleSpeed);

    for (auto& star : gameState->getStars()) {
        if (star->getPlayer() == gameState->getHumanPlayer()) {
            viewPort.x = star->getPosition().getX() * -1 + (renderer->getMainCamera()->getWidth() / 2);
            viewPort.y = star->getPosition().getY() * -1 + (renderer->getMainCamera()->getHeight() / 2);
            std::cout << star->getName() << std::endl;
            break;
        }
    }
}

void StarMapScene::renderUI()
{

    planetWindow.render(renderer, uiTexture);

    std::time_t tmpTime = std::chrono::system_clock::to_time_t(
        timeThread->getTime());

    char date_time_format[] = "%d.%m.%Y";

    char time_str[100];

    std::strftime(time_str, 100, date_time_format,
        std::localtime(&tmpTime));

    int xLeft = 10;
    int yLeft = 10;
    SDL_Color color = { 255, 255, 0, 255 };
    //cash
    glyphText.render(renderer, "\uf51e", color, xLeft, yLeft);
    xLeft += 25;
    uiText.render(renderer, utils::string_format("%d [%+d]", gameState->getHumanPlayer()->getMoney(), gameState->getHumanPlayer()->getMoneyPerMonth()), color, xLeft, yLeft + 3);

    //research
    xLeft += 80;
    color = { 0, 200, 200, 255 };
    glyphText.render(renderer, "\uf0c3", color, xLeft, yLeft);
    xLeft += 25;
    uiText.render(renderer, utils::string_format("%+d", gameState->getHumanPlayer()->getResearchPerMonth()), color, xLeft, yLeft + 3);
    xLeft += 80;
    color = { 100, 200, 0, 255 };
    glyphText.render(renderer, "\uf017", color, xLeft, yLeft);
    xLeft += 25;
    uiText.render(renderer, std::string(time_str), color, xLeft, yLeft + 3);
    container->render(renderer);
}

void StarMapScene::handleEvents(core::Input* pInput)
{
    if (planetWindow.getVisible()) {
        planetWindow.handleEvents(pInput);
        return;
    }

    const float MAX_SPEED = 400.0f;

    if (pInput->isKeyDown(SDLK_DOWN)) {
        if (mouseSpeedY < MAX_SPEED)
            mouseSpeedY++;
        viewPort.y -= 3 * mouseSpeedY / 100.0f;
    } else if (pInput->isKeyDown(SDLK_UP)) {
        if (mouseSpeedY < MAX_SPEED)
            mouseSpeedY++;
        viewPort.y += 3 * mouseSpeedY / 100.0f;
    } else {
        mouseSpeedY = 100;
    }

    if (pInput->isKeyDown(SDLK_LEFT)) {
        if (mouseSpeedX < MAX_SPEED)
            mouseSpeedX++;
        viewPort.x += 3 * mouseSpeedX / 100.0f;
    } else if (pInput->isKeyDown(SDLK_RIGHT)) {
        if (mouseSpeedX < MAX_SPEED)
            mouseSpeedX++;
        viewPort.x -= 3 * mouseSpeedX / 100.0f;
    } else {
        mouseSpeedX = 100;
    }

    if (pInput->isScrollWheel()) {

        utils::Vector2 pos = pInput->getMouseWheelPosition();
        renderer->setZoomFactor(renderer->getZoomFactor() + pos.getY() / 100.0f);

        viewPort.width = WORLD_SIZE * renderer->getZoomFactor();
        viewPort.height = WORLD_SIZE * renderer->getZoomFactor();
        //viewPort.x *= renderer->getZoomFactor();
        //viewPort.y *= renderer->getZoomFactor();
    }

    //detect click on planet
    graphics::Rect planetRect;
    utils::Vector2 mousePos = pInput->getMousePostion();

    for (const auto& star : gameState->getStars()) {
        float x = star->getPosition().getX();
        float y = star->getPosition().getY();
        int i = 1;
        for (const auto& planet : star->getPlanets()) {
            int planetX = std::cos(planet->getAngle() * PI / 180.0)
                * (i * PLANET_DISTANCE);
            int planetY = std::sin(planet->getAngle() * PI / 180.0)
                * (i * PLANET_DISTANCE);
            int tmpx = (planet->getSize() * 3 * std::cos(135 * PI / 180.0)) / 2;
            int tmpy = (planet->getSize() * 3 * std::sin(135 * PI / 180.0)) / 2;
            planetRect.x = x + planetX + tmpx;

            planetRect.y = y + (planetY + tmpy) * -1;
            planetRect.width = planet->getSize() * 3;
            planetRect.height = planet->getSize() * 3;
            planetRect.x *= renderer->getZoomFactor();
            planetRect.y *= renderer->getZoomFactor();
            planetRect.width *= renderer->getZoomFactor();
            planetRect.height *= renderer->getZoomFactor();
            planetRect.x += viewPort.x;
            planetRect.y += viewPort.y;
            i++;

            if (planetRect.intersects(mousePos)) {
                utils::Vector2 planetVec(planetRect.x + (planetRect.width * renderer->getZoomFactor() / 2), planetRect.y + (planetRect.height * renderer->getZoomFactor() / 2));
                //mouse hover planet
                targetFleetVec = planetVec;

                if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
                    if (selectedFleet != nullptr) {
                        float px = selectedFleet->getPosition().getX() * renderer->getZoomFactor();
                        float py = selectedFleet->getPosition().getY() * renderer->getZoomFactor();
                        float width = selectedFleet->getFirstShip()->getTexture()->getWidth() * 0.2f * renderer->getZoomFactor();
                        float height = selectedFleet->getFirstShip()->getTexture()->getHeight() * 0.2f * renderer->getZoomFactor();
                        px += (width / 2.f);
                        py += (height / 2.f);

                        auto endPos = utils::Vector2(x + planetX + tmpx, y + (planetY + tmpy) * -1);
                        utils::Vector2 start(px, py);

                        if (selectedFleet->getFirstShip()->isDistanceReachable(endPos.distance(selectedFleet->getPosition()) / renderer->getZoomFactor())) {
                            selectedFleet->setStartPosition(selectedFleet->getPosition());
                            //selectedFleet->setPosition(start);
                            selectedFleet->setTargetPosition(endPos);
                        }
                    } else {
                        std::cout << "user clicked on planet " << planet->getName()
                                  << std::endl;
                        planetWindow.setPlanet(planet);
                        planetWindow.setPlayer(gameState->getHumanPlayer());
                        planetWindow.setVisible(true);
                    }
                }
            }
        }
    }
    for (auto& fleet : gameState->getFleets()) {
        auto ship = fleet->getFirstShip();
        auto texture = ship->getTexture();
        graphics::Rect fleetRect;
        fleetRect.x = fleet->getPosition().getX() * renderer->getZoomFactor();
        fleetRect.y = fleet->getPosition().getY() * renderer->getZoomFactor();
        fleetRect.width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
        fleetRect.height = texture->getHeight() * 0.2f * renderer->getZoomFactor();
        fleetRect.x += viewPort.x;
        fleetRect.y += viewPort.y;
        if (fleetRect.intersects(mousePos)) {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
                selectedFleet = fleet;
            } else if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT)) {
                selectedFleet = nullptr;
            }
        }
    }
    container->handleEvents(pInput);
}

void StarMapScene::render()
{
    // render background
    background.render(renderer, 0, 0);
    renderer->setViewPort(viewPort);
    graphics::Rect planetRect;
    SDL_Color ellipseColor = { 192, 192, 192, 255 };
    // render stars
    for (auto& star : gameState->getStars()) {
        int x = star->getPosition().getX();
        int y = star->getPosition().getY();
        SDL_Color color = { 255, 255, 255, 0 };
        //render star image
        auto player = star->getPlayer();
        if (player != nullptr) {
            color = player->getColor();
        }
        //render star
        starTextures[star->getType()]->renderResized(renderer, (x - 25) * renderer->getZoomFactor(), (y - 25) * renderer->getZoomFactor(),
            50 * renderer->getZoomFactor(), 50 * renderer->getZoomFactor());
        //render star name
        int w = 0;
        int h = 0;
        starText.size(star->getName(), &w, &h);
        starText.render(renderer, star->getName(), color, (x - w / 2) * renderer->getZoomFactor(), (y + 30) * renderer->getZoomFactor());

        //render planets
        int i = 1;

        for (auto& planet : star->getPlanets()) {

            //planet->getType()

            int planetX = std::cos(planet->getAngle() * PI / 180.0)
                * (i * PLANET_DISTANCE);
            int planetY = std::sin(planet->getAngle() * PI / 180.0)
                * (i * PLANET_DISTANCE);

            renderer->drawCircle(x * renderer->getZoomFactor(), y * renderer->getZoomFactor(), i * 50 * renderer->getZoomFactor(),
                ellipseColor);
            int tmpx = (planet->getSize() * 3.0 * std::cos(135.0 * PI / 180.0)) / 2.0;
            int tmpy = (planet->getSize() * 3.0 * std::sin(135.0 * PI / 180.0)) / 2.0;
            planetTextures[planet->getType()]->renderResized(renderer,
                (planetX + x + tmpx) * renderer->getZoomFactor(), (y + (planetY + tmpy) * -1) * renderer->getZoomFactor(),
                planet->getSize() * 3 * renderer->getZoomFactor(), planet->getSize() * 3 * renderer->getZoomFactor());

            i++;
            //planet->renderPlanetSurface(renderer, &planetSurfaceTexture);
        }

        //render relations
    }
    //render ships
    for (auto& fleet : gameState->getFleets()) {
        auto ship = fleet->getFirstShip();
        auto texture = ship->getTexture();

        int x = fleet->getPosition().getX() * renderer->getZoomFactor();
        int y = fleet->getPosition().getY() * renderer->getZoomFactor();
        int width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
        int height = texture->getHeight() * 0.2f * renderer->getZoomFactor();

        texture->renderRotated(renderer, 90, x, y, width, height);
    }
    if (selectedFleet != nullptr) {
        auto startPos = selectedFleet->getPosition();
        auto ship = selectedFleet->getFirstShip();
        auto texture = ship->getTexture();
        float x = selectedFleet->getPosition().getX() * renderer->getZoomFactor();
        float y = selectedFleet->getPosition().getY() * renderer->getZoomFactor();
        float width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
        float height = texture->getHeight() * 0.2f * renderer->getZoomFactor();
        x += (width / 2);
        y += (height / 2);

        //x -= viewPort.x;
        //y -= viewPort.y;
        auto endPos = utils::Vector2(targetFleetVec.getX() - viewPort.x, targetFleetVec.getY() - viewPort.y);
        utils::Vector2 start(x, y);

        std::cout << "distinace: " << endPos.distance(start) / renderer->getZoomFactor() << std::endl;
        if (ship->isDistanceReachable(endPos.distance(start) / renderer->getZoomFactor())) {
            renderer->setDrawColor(0, 255, 0, 255);
        } else {
            renderer->setDrawColor(255, 0, 0, 255);
        }

        renderer->drawLine(start, endPos);
    }

    renderer->setViewPort(defaultViewPort);
    //render ui
    renderUI();
}

} /* namespace scenes */
