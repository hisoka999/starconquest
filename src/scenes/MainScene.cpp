/*
 * MainScene.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "MainScene.h"
#include "../Star.h"
#include "../WorldGenerator.h"
#include "../services/buildingservice.h"
#include "../services/researchservice.h"
#include "../translate.h"
#include "NewGameScene.h"
#include <engine/graphics/TextureManager.h>
#include <engine/ui/Button.h>
#include <engine/utils/os.h>
#include <functional>
#include <iostream>
#include <vector>
#include "StarMapScene.h"

namespace scenes
{

    MainScene::MainScene(core::Renderer *pRenderer,
                         core::SceneManager *pSceneManager, const core::GameWindow *gameWindow)
        : core::Scene(pRenderer), running(true),
          settingsWindow(gameWindow)
    {
        sceneManager = pSceneManager;
        bgTexture = graphics::TextureManager::Instance().loadTexture("images/title_background.png");

        container = std::make_shared<UI::Container>();
        SDL_Color white = {255, 255, 255, 0};
        std::shared_ptr<UI::Button> btnStart = std::make_shared<UI::Button>();
        btnStart->setFont("fonts/Audiowide-Regular.ttf", 14);
        btnStart->setColor(white);
        btnStart->setLabel(_("New Game"));
        btnStart->setPos(450, 350);
        btnStart->setStaticWidth(150);
        container->addObject(btnStart);

        btnStart->connect(UI::Button::buttonClickCallback(), [&]() { startGame(); });

        std::shared_ptr<UI::Button> btnLoadGame = std::make_shared<UI::Button>();
        btnLoadGame->setFont("fonts/Audiowide-Regular.ttf", 14);
        btnLoadGame->setColor(white);
        btnLoadGame->setLabel(_("Load Game"));
        btnLoadGame->setPos(450, 400);
        btnLoadGame->setStaticWidth(150);
        btnLoadGame->disable();

        btnLoadGame->connect(UI::Button::buttonClickCallback(), [&]() { loadGame(); });

        container->addObject(btnLoadGame);

        std::shared_ptr<UI::Button> btnSettingsGame = std::make_shared<UI::Button>();
        btnSettingsGame->setFont("fonts/Audiowide-Regular.ttf", 14);
        btnSettingsGame->setColor(white);
        btnSettingsGame->setLabel(_("Settings"));
        btnSettingsGame->setPos(450, 450);
        btnSettingsGame->setStaticWidth(150);

        btnSettingsGame->connect(UI::Button::buttonClickCallback(), [&]() { settingsWindow.setVisible(true); });

        container->addObject(btnSettingsGame);

        std::shared_ptr<UI::Button> btnExit = std::make_shared<UI::Button>();
        btnExit->setFont("fonts/Audiowide-Regular.ttf", 14);
        btnExit->setColor(white);
        btnExit->setLabel(_("Exit Game"));
        btnExit->setPos(450, 500);
        btnExit->setStaticWidth(150);

        btnExit->connect(UI::Button::buttonClickCallback(), [&]() { exitGame(); });

        container->addObject(btnExit);
    }
    void MainScene::render()
    {
        bgTexture->renderResized(renderer, 0, 0,
                                 renderer->getMainCamera()->getWidth(),
                                 renderer->getMainCamera()->getHeight());

        container->render(renderer);
        settingsWindow.render(renderer);
    }

    void MainScene::exitGame()
    {
        running = false;
    }
    std::vector<std::shared_ptr<Building>> MainScene::initBuildings()
    {
        std::vector<std::shared_ptr<Building>> buildings = BuildingService::Instance().getData();

        return buildings;
    }
    void MainScene::startGame()
    {

        auto newGameScene = std::make_shared<NewGameScene>(renderer, sceneManager);
        sceneManager->addScene("newGame", newGameScene);
        sceneManager->setCurrentScene("newGame");
    }

    void MainScene::loadGame()
    {
    }

    void MainScene::handleEvents(core::Input *pInput)
    {
        container->handleEvents(pInput);
        settingsWindow.handleEvents(pInput);
    }

    MainScene::~MainScene()
    {
        bgTexture = nullptr;
    }

} /* namespace scenes */
