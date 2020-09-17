//============================================================================
// Name        : starconquest.cpp
// Author      : Stefan Lüdtke
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

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
#include <boost/locale.hpp>
#include <iostream>

using namespace boost::locale;

int main()
{

    generator gen;

    // Specify location of dictionaries
    gen.add_messages_path("locale");
    gen.add_messages_domain("starconquest");
    try {
        // Generate locales and imbue them to iostream
        std::locale::global(gen(""));
        std::cout.imbue(std::locale());

        core::GameWindow win("Star Conquest", 1280, 720);
        utils::Logger logger(utils::LogLevel::trace);
        core::Renderer ren(logger);
        graphics::TextureManager::Instance().setRenderer(&ren);
        core::Input input;

        core::SceneManager sceneManager;
        win.open();
        ren.open(&win, true);
        graphics::Rect viewPort = ren.getViewPort();
        core::Camera mainCamera(viewPort);
        ren.setMainCamera(&mainCamera);
        scenes::MainScene mainScene(&ren, &sceneManager);
        sceneManager.addScene("main", &mainScene);
        sceneManager.setCurrentScene("main");

        unsigned int lastTime = ren.getTickCount();
        unsigned int frames = 0;
        unsigned int fps = 0;
        graphics::Text text;
        text.openFont(utils::os::combine("fonts", "arial.ttf"), 22);
        SDL_Color color = { 200, 200, 0, 0 };
        // game loop
        unsigned int delay = 0;

        bool run = true;
        while (run && mainScene.isRunning()) {
            ren.clear();
            try {
                while (input.poll()) {
                    if (input.isKeyDown(SDLK_ESCAPE) || input.isQuit())
                        run = false;
                    sceneManager.handleEvents(&input);
                }
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            sceneManager.render();
            frames++;
            if ((ren.getTickCount() - lastTime) >= 1000) {
                lastTime = ren.getTickCount();
                fps = frames;
                frames = 0;
                if (fps > 300) {
                    delay++;
                    std::cout << "delay => " << delay << std::endl;
                } else {
                    if (delay > 0)
                        delay--;
                }
            }

            text.render(&ren, "FPS: " + std::to_string(fps), color, 850, 5);
            ren.renderPresent();

            win.delay(delay);
            ren.calcDelta();
        }
    } catch (SDLException& e) {
        std::cerr << "SDL Exception: " << e.what() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "unkown standard exception: " << e.what() << std::endl;
    }
    return 0;
}
