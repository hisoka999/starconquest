/*
 * StarMapScene.cpp
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#include "StarMapScene.h"
#include "../constants.h"
#include "../translate.h"
#include <cmath>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/iconbutton.h>
#include <engine/utils/os.h>

namespace scenes
{

    StarMapScene::~StarMapScene()
    {
        timeThread->stop();
        timeThread = nullptr;
        gameState = nullptr;
    }

    StarMapScene::StarMapScene(core::Renderer *pRenderer, std::shared_ptr<GameState> gameState)
        : core::Scene(pRenderer), selectedFleet(nullptr), mouseSpeedX(100), mouseSpeedY(100), researchWindow(gameState->getHumanPlayer())
    {

        winMgr->addWindow(&researchWindow);

        researchWindow.setPos(100, 100);
        direction = {false, false, false, false};

        this->gameState = gameState;
        auto colonyShip = std::make_shared<Ship>("ColonyShip", "Kolonieschiff", 500, ShipType::ColonyShip, 1);

        auto playerStars = gameState->findStarsForPlayer(gameState->getHumanPlayer());
        auto playerPlanet = playerStars[0]->getPlanets()[0];

        colonyShip->loadTexture(utils::os::combine("images", "ships", "colonyship.png"));

        colonyShip->addAttribute(Attribute::Hull, 10);
        colonyShip->addAttribute(Attribute::Drive, 10);
        auto fleet = std::make_shared<Fleet>("Fleet 1");
        fleet->setOwner(gameState->getHumanPlayer().get());
        fleet->addShip(colonyShip);
        fleet->setPosition(playerStars[0]->getPosition());
        fleet->setTargetPosition(playerStars[0]->getPosition());
        gameState->addFleet(fleet);

        timeThread = std::make_unique<TimeThread>(gameState);
        starText.openFont(utils::os::combine("fonts", "Audiowide-Regular.ttf"), 12);
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
            timeThread->start();
            timeThread->setSpeed(100);
        });

        int xLeft = 10;
        int yLeft = 0;
        SDL_Color color = {255, 255, 0, 255};
        //cash
        cashButton = std::make_shared<UI::IconButton>();
        cashButton->setFont("fonts/Audiowide-Regular.ttf", 12);
        cashButton->setLabel("");
        cashButton->setIconText("\uf51e");
        cashButton->setColor(color);
        cashButton->setPos(xLeft, yLeft);
        cashButton->setBorderless(true);
        xLeft += 25;

        //research
        xLeft += 80;
        color = {0, 200, 200, 255};

        researchButton = std::make_shared<UI::IconButton>();
        researchButton->setFont("fonts/Audiowide-Regular.ttf", 12);
        researchButton->setBorderless(true);
        researchButton->setLabel("");
        researchButton->setIconText("\uf0c3");
        researchButton->setColor(color);
        researchButton->setPos(xLeft, yLeft);
        researchButton->connect(UI::Button::buttonClickCallback(), [=]() {
            researchWindow.setVisible(true);
        });

        container->addObject(cashButton);
        container->addObject(researchButton);

        container->addObject(playButton);
        container->addObject(pauseButton);
        container->addObject(doubleSpeed);
        renderer->getMainCamera()->reset();

        for (auto &star : gameState->getStars())
        {
            if (star->getPlayer() == gameState->getHumanPlayer())
            {
                float camX = star->getPosition().getX() - (renderer->getMainCamera()->getWidth() / 2);
                float camY = star->getPosition().getY() - (renderer->getMainCamera()->getHeight() / 2);

                renderer->getMainCamera()->move(camX, camY);
                std::cout << "Player star: " << star->getName() << std::endl;
                std::cout << "viewPort.x " << viewPort.x << std::endl;
                std::cout << "viewPort.y " << viewPort.y << std::endl;
                break;
            }
        }
    }

    void StarMapScene::renderUI()
    {

        planetWindow.render(renderer, uiTexture);
        winMgr->render(renderer);

        std::time_t tmpTime = std::chrono::system_clock::to_time_t(
            timeThread->getTime());

        char date_time_format[] = "%d.%m.%Y";

        char time_str[100];

        std::strftime(time_str, 100, date_time_format,
                      std::localtime(&tmpTime));

        //render time
        int xLeft = 220;
        int yLeft = 10;
        SDL_Color color = {100, 200, 0, 255};

        glyphText.render(renderer, "\uf017", color, xLeft, yLeft);
        xLeft += 25;
        uiText.render(renderer, std::string(time_str), color, xLeft, yLeft + 3);
        container->render(renderer);
    }

    void StarMapScene::handleEvents(core::Input *pInput)
    {
        if (planetWindow.getVisible())
        {
            planetWindow.handleEvents(pInput);
            return;
        }

        if (winMgr->isWindowOpen())
        {
            winMgr->handleInput(pInput);
            return;
        }

        if (pInput->isKeyDown(SDLK_DOWN))
        {
            direction.bottom = true;
            direction.top = false;
        }
        else if (pInput->isKeyDown(SDLK_UP))
        {
            direction.top = true;
            direction.bottom = false;
        }
        else
        {
            direction.top = false;
            direction.bottom = false;
        }

        if (pInput->isKeyDown(SDLK_LEFT))
        {
            direction.left = true;
            direction.right = false;
        }
        else if (pInput->isKeyDown(SDLK_RIGHT))
        {
            direction.left = false;
            direction.right = true;
        }
        else
        {
            direction.left = false;
            direction.right = false;
        }

        if (utils::areSame(pInput->getMousePostion().getX(), 0.f))
        {
            direction.left = true;
            direction.right = false;
        }
        else if (renderer->getMainCamera()->getWidth() - pInput->getMousePostion().getX() <= 5)
        {
            direction.left = false;
            direction.right = true;
        }

        if (utils::areSame(pInput->getMousePostion().getY(), 0.f))
        {
            direction.top = true;
            direction.bottom = false;
        }
        else if (renderer->getMainCamera()->getHeight() - pInput->getMousePostion().getY() <= 5)
        {
            direction.top = false;
            direction.bottom = true;
        }

        if (pInput->isScrollWheel())
        {

            utils::Vector2 pos = pInput->getMouseWheelPosition();
            float oldFactor = renderer->getZoomFactor();
            renderer->setZoomFactor(oldFactor + pos.getY() / 100.0f);

            float diffX = WORLD_SIZE - (WORLD_SIZE * (1 - (pos.getY() / 100.0f)));
            float diffY = WORLD_SIZE - (WORLD_SIZE * (1 - (pos.getY() / 100.0f)));

            renderer->getMainCamera()->move(diffX / 2.f, diffY / 2.f);
            std::cout << "diff " << diffX << " : " << diffY << std::endl;
            std::cout << "old:" << oldFactor << "zoom: " << renderer->getZoomFactor() << std::endl;
            std::cout << "cam x " << renderer->getMainCamera()->getX() << " cam y " << renderer->getMainCamera()->getY() << std::endl;
        }
        float cameraX = renderer->getMainCamera()->getX();
        float cameraY = renderer->getMainCamera()->getY();

        //detect click on planet
        graphics::Rect planetRect;
        utils::Vector2 mousePos = pInput->getMousePostion();

        for (const auto &star : gameState->getStars())
        {
            float x = star->getPosition().getX();
            float y = star->getPosition().getY();
            int i = 1;

            graphics::Rect starRect = {(x - 25) * renderer->getZoomFactor(), (y - 25) * renderer->getZoomFactor(),
                                       50 * renderer->getZoomFactor(), 50 * renderer->getZoomFactor()};
            starRect.x -= cameraX;
            starRect.y -= cameraY;
            if (starRect.intersects(mousePos) && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                viewPort.width = WORLD_SIZE;
                viewPort.height = WORLD_SIZE;
                renderer->setZoomFactor(1);
                renderer->getMainCamera()->reset();
                renderer->getMainCamera()->move(star->getPosition().getX() - (renderer->getMainCamera()->getWidth() / 2), star->getPosition().getY() - (renderer->getMainCamera()->getHeight() / 2));
            }

            for (const auto &planet : star->getPlanets())
            {
                float planetX = std::cos(planet->getAngle() * PI / 180.0f) * (i * PLANET_DISTANCE);
                float planetY = std::sin(planet->getAngle() * PI / 180.0f) * (i * PLANET_DISTANCE);
                float tmpx = (planet->getSize() * 3 * std::cos(135 * PI / 180.0f)) / 2;
                float tmpy = (planet->getSize() * 3 * std::sin(135 * PI / 180.0f)) / 2;
                planetRect.x = x + planetX + tmpx;

                planetRect.y = y + (planetY + tmpy) * -1;
                planetRect.width = planet->getSize() * 3;
                planetRect.height = planet->getSize() * 3;
                planetRect.x *= renderer->getZoomFactor();
                planetRect.y *= renderer->getZoomFactor();
                planetRect.width *= renderer->getZoomFactor();
                planetRect.height *= renderer->getZoomFactor();
                planetRect.x -= cameraX;
                planetRect.y -= cameraY;
                i++;

                if (planetRect.intersects(mousePos))
                {
                    utils::Vector2 planetVec(planetRect.x + (planetRect.width * renderer->getZoomFactor() / 2), planetRect.y + (planetRect.height * renderer->getZoomFactor() / 2));
                    //mouse hover planet
                    targetFleetVec = planetVec;

                    if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
                    {
                        if (selectedFleet != nullptr)
                        {
                            float px = selectedFleet->getPosition().getX() * renderer->getZoomFactor();
                            float py = selectedFleet->getPosition().getY() * renderer->getZoomFactor();
                            float width = selectedFleet->getFirstShip()->getTexture()->getWidth() * 0.2f * renderer->getZoomFactor();
                            float height = selectedFleet->getFirstShip()->getTexture()->getHeight() * 0.2f * renderer->getZoomFactor();
                            px += (width / 2.f);
                            py += (height / 2.f);

                            auto endPos = utils::Vector2(x + planetX + tmpx, y + (planetY + tmpy) * -1);
                            utils::Vector2 start(px, py);

                            if (selectedFleet->getFirstShip()->isDistanceReachable(endPos.distance(selectedFleet->getPosition()) / renderer->getZoomFactor()))
                            {
                                selectedFleet->setStartPosition(selectedFleet->getPosition());
                                //selectedFleet->setPosition(start);
                                selectedFleet->setTargetPosition(endPos);
                            }
                        }
                        else
                        {
                            std::cout << "user clicked on planet " << planet->getName()
                                      << std::endl;
                            planetWindow.setGameState(gameState);
                            planetWindow.openForPlanet(star, planet);
                        }
                    }
                }
            }
        }
        for (auto &fleet : gameState->getFleets())
        {
            auto ship = fleet->getFirstShip();
            auto texture = ship->getTexture();
            graphics::Rect fleetRect;
            fleetRect.x = fleet->getPosition().getX() * renderer->getZoomFactor();
            fleetRect.y = fleet->getPosition().getY() * renderer->getZoomFactor();
            fleetRect.width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
            fleetRect.height = texture->getHeight() * 0.2f * renderer->getZoomFactor();
            fleetRect.x -= cameraX;
            fleetRect.y -= cameraY;
            if (fleetRect.intersects(mousePos))
            {
                if (pInput->isMouseDoubleClick())
                {
                    selectedFleet = fleet;
                    std::cout << "open fleet window" << std::endl;
                    if (fleetWindows.count(selectedFleet->getName()) == 0)
                    {
                        auto fleetWindow = std::make_shared<windows::FleetWindow>();
                        fleetWindows[selectedFleet->getName()] = fleetWindow;
                        winMgr->addWindow(fleetWindow.get());
                    }
                    fleetWindows[selectedFleet->getName()]->setPos(pInput->getMousePostion().getX(), pInput->getMousePostion().getY());
                    fleetWindows[selectedFleet->getName()]->setVisible(true);
                    fleetWindows[selectedFleet->getName()]->setFleet(fleet);
                }
                else if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
                {
                    selectedFleet = fleet;
                }
            }
        }
        if (pInput->isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            selectedFleet = nullptr;
        }
        container->handleEvents(pInput);
    }

    void StarMapScene::render()
    {
        // render background
        background.render(renderer, 0, 0);
        renderer->setViewPort(viewPort);
        float cameraX = renderer->getMainCamera()->getX();
        float cameraY = renderer->getMainCamera()->getY();

        graphics::Rect planetRect;
        SDL_Color ellipseColor = {192, 192, 192, 255};
        // render stars
        for (auto &star : gameState->getStars())
        {
            float x = star->getPosition().getX();
            float y = star->getPosition().getY();
            utils::Vector2 resizedPosition = star->getPosition() * renderer->getZoomFactor();
            if (!renderer->getMainCamera()->getViewPortRect().intersects(resizedPosition))
                continue;
            SDL_Color color = {255, 255, 255, 0};
            //render star image
            auto player = star->getPlayer();
            if (player != nullptr)
            {
                color = player->getColor();
            }
            //render star
            float starX = (x - 25) * renderer->getZoomFactor();
            float starY = (y - 25) * renderer->getZoomFactor();
            starX -= cameraX;
            starY -= cameraY;
            starTextures[star->getType()]->renderResized(renderer, starX, starY,
                                                         50 * renderer->getZoomFactor(), 50 * renderer->getZoomFactor());
            //render star name
            int w = 0;
            int h = 0;
            starText.size(star->getName(), &w, &h);
            starText.render(renderer, star->getName(), color, ((x - w / 2) * renderer->getZoomFactor()) - cameraX, ((y + 30) * renderer->getZoomFactor()) - cameraY);

            //render planets
            int i = 1;
            if (renderer->getZoomFactor() >= 0.25f)
            {

                for (auto &planet : star->getPlanets())
                {

                    //planet->getType()

                    float planetX = std::cos(planet->getAngle() * PI / 180.0f) * (static_cast<float>(i) * PLANET_DISTANCE);
                    float planetY = std::sin(planet->getAngle() * PI / 180.0f) * (i * PLANET_DISTANCE);

                    renderer->drawCircle((x * renderer->getZoomFactor()) - cameraX, (y * renderer->getZoomFactor()) - cameraY, i * 50.f * renderer->getZoomFactor(),
                                         ellipseColor);
                    float tmpx = (planet->getSize() * 3.0f * std::cos(135.0f * PI / 180.0f)) / 2.0f;
                    float tmpy = (planet->getSize() * 3.0f * std::sin(135.0f * PI / 180.0f)) / 2.0f;
                    planetX += x + tmpx;
                    planetY = y + (planetY + tmpy) * -1;
                    planetX *= renderer->getZoomFactor();
                    planetY *= renderer->getZoomFactor();
                    planetX -= cameraX;
                    planetY -= cameraY;

                    planetTextures[planet->getType()]->renderResized(renderer,
                                                                     planetX, planetY,
                                                                     planet->getSize() * 3 * renderer->getZoomFactor(), planet->getSize() * 3 * renderer->getZoomFactor());

                    i++;
                    //planet->renderPlanetSurface(renderer, &planetSurfaceTexture);
                }
            }
            //render relations
        }
        //render ships
        for (const auto &fleet : gameState->getFleets())
        {
            auto ship = fleet->getFirstShip();
            auto texture = ship->getTexture();

            int x = fleet->getPosition().getX() * renderer->getZoomFactor();
            int y = fleet->getPosition().getY() * renderer->getZoomFactor();
            int width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
            int height = texture->getHeight() * 0.2f * renderer->getZoomFactor();
            x -= cameraX;
            y -= cameraY;

            //
            float rotation = 90;

            if (fleet->getTargetPosition() != fleet->getPosition())
            {
                utils::Vector2 vecMin(fleet->getPosition().getX(), fleet->getPosition().getY() - PI);

                float b = fleet->getTargetPosition().distance(fleet->getPosition());
                float a = PI;
                float c = vecMin.distance(fleet->getTargetPosition());

                rotation = std::acos((std::pow(a, 2.f) + std::pow(b, 2.f) - std::pow(c, 2.f)) / (2 * a * b)) * 180.f / PI;
                if (fleet->getTargetPosition().getX() < fleet->getPosition().getX())
                {
                    rotation = 360.f - rotation;
                }
            }

            texture->renderRotated(renderer, rotation, x, y, width, height);
        }
        if (selectedFleet != nullptr)
        {
            if (selectedFleet->getShips().empty())
                selectedFleet = nullptr;
        }

        if (selectedFleet != nullptr)
        {
            auto startPos = selectedFleet->getPosition();
            auto ship = selectedFleet->getFirstShip();
            auto texture = ship->getTexture();
            float x = selectedFleet->getPosition().getX() * renderer->getZoomFactor();
            float y = selectedFleet->getPosition().getY() * renderer->getZoomFactor();
            float width = texture->getWidth() * 0.2f * renderer->getZoomFactor();
            float height = texture->getHeight() * 0.2f * renderer->getZoomFactor();
            x += (width / 2);
            y += (height / 2);

            x -= cameraX;
            y -= cameraY;

            //x -= viewPort.x;
            //y -= viewPort.y;
            auto endPos = utils::Vector2(targetFleetVec.getX(), targetFleetVec.getY());
            utils::Vector2 start(x, y);

            //std::cout << "distinace: " << endPos.distance(start) / renderer->getZoomFactor() << std::endl;
            if (ship->isDistanceReachable(endPos.distance(start) / renderer->getZoomFactor()))
            {
                renderer->setDrawColor(0, 255, 0, 255);
            }
            else
            {
                renderer->setDrawColor(255, 0, 0, 255);
            }

            renderer->drawLine(start, endPos);
        }

        renderer->setViewPort(defaultViewPort);
        //render ui
        renderUI();
    }

    void StarMapScene::update()
    {
        if (!timeThread->getPaused())
            gameState->updateFleetPosition(renderer->getTimeDelta() * (800.f - timeThread->getSpeed()) / 800.f);

        float speed = renderer->getTimeDelta() / 1000.f * 400.f;

        float moveX = 0.f;
        float moveY = 0.f;

        if (direction.top)
        {
            //if (viewPort.y > 0)
            moveY -= speed;
        }
        else if (direction.bottom)
        {
            moveY += speed;
        }

        if (direction.left)
        {
            //if (viewPort.x > 0)
            moveX -= speed;
        }
        else if (direction.right)
        {
            moveX += speed;
        }
        renderer->getMainCamera()->move(moveX, moveY);

        //update UI-Text
        cashButton->setLabel(utils::string_format("%d [%+d]", gameState->getHumanPlayer()->getMoney(), gameState->getHumanPlayer()->getMoneyPerMonth()));
        researchButton->setLabel(utils::string_format("%+d", gameState->getHumanPlayer()->getResearchPerMonth()));

        graphics::TextureManager::Instance().updateRessources();
    }
} /* namespace scenes */
