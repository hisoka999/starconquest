//============================================================================
// Name        : starconquest.cpp
// Author      : Stefan Lüdtke
//============================================================================

#ifdef _WIN32
#include <windows.h>
#endif
#include "engine/core/SceneManager.h"
#include "engine/core/gamewindow.h"
#include "engine/core/input.h"
#include "engine/core/renderer.h"
#include "engine/graphics/TextureManager.h"
#include "engine/graphics/text.h"
#include "engine/graphics/texture.h"
#include "engine/utils/exceptions.h"
#include "engine/utils/logger.h"
#include "engine/utils/os.h"
#include "scenes/MainScene.h"
#include "services/buildingservice.h"
#include "services/researchservice.h"
#include "services/shipservice.h"
#include "services/raceservice.h"
#include "translate.h"
#include <engine/utils/json/parser.h>
#include <iostream>

//#ifdef _WIN32
//INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//          PSTR lpCmdLine, INT nCmdShow)
//#else
int main(int argc, char *argv[])

//#endif
{

    Localisation::Instance().detectLanguage();

    setlocale(LC_ALL, "");
    core::GameWindow win("Star Conquest", 1280, 720);
    utils::Logger logger(utils::LogLevel::trace);
    core::Renderer ren(logger);
    logger.trace(__FILE__, "renderer initialized");
    graphics::TextureManager::Instance().setRenderer(&ren);
    core::Input input;
    try
    {

        auto &sceneManager = core::SceneManager::Instance();
        win.open();
        ren.open(&win, false);
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);
        auto mainScene = std::make_shared<scenes::MainScene>(&ren, &sceneManager, &win);
        sceneManager.addScene("main", mainScene);
        sceneManager.setCurrentScene("main");

        ShipService::Instance().loadData("data/ships.json");
        BuildingService::Instance().loadData("data/buildings.json");
        ResearchService::Instance().loadResearch("data/research.json");
        RaceService::Instance().loadData("data/races.json");

        unsigned int lastTime = ren.getTickCount();
        unsigned int frames = 0;
        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color = {200, 200, 0, 0};
        // game loop
        unsigned int delay = 0;

        bool run = true;
        while (run && mainScene->isRunning())
        {
            ren.clear();
            try
            {
                while (input.poll())
                {
                    if (input.isQuit())
                        run = false;
                    else if (input.isKeyDown(SDLK_ESCAPE))
                    {
                        sceneManager.setCurrentScene("main");
                    }
                    sceneManager.handleEvents(&input);
                }
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            sceneManager.update();
            sceneManager.render();
            frames++;
            if ((ren.getTickCount() - lastTime) >= 1000)
            {
                lastTime = ren.getTickCount();
                fps = frames;
                frames = 0;
                //                if (fps > 300) {
                //                    delay++;
                //                    std::cout << "delay => " << delay << std::endl;
                //                } else {
                //                    if (delay > 0)
                //                        delay--;
                //                }
            }

            text.render(&ren, "FPS: " + std::to_string(fps), color, mainCamera.getWidth() - 100.f, 5);
            ren.renderPresent();

            //win.delay(delay);
            ren.calcDelta();
        }
        core::SceneManager::free();
        graphics::TextureManager::free();
    }
    catch (SDLException &e)
    {
        std::cout << "SDL Exception: " << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "unkown standard exception: " << e.what() << std::endl;
    }

    return 0;
}
