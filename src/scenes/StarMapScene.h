/*
 * StarMapScene.h
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#ifndef SCENES_STARMAPSCENE_H_
#define SCENES_STARMAPSCENE_H_

#include "../Player.h"
#include "../Star.h"
#include "../TimeThread.h"
#include "../windows/PlanetWindow.h"
#include "../windows/fleetwindow.h"
#include "../windows/researchwindow.h"
#include <engine/core/Scene.h>
#include <engine/core/renderer.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <map>

namespace UI
{
    class IconButton;
}

namespace scenes
{

    struct MovementDirection
    {
        bool left = false;
        bool right = false;
        bool top = false;
        bool bottom = false;
    };

    class StarMapScene : public core::Scene
    {
    public:
        StarMapScene(core::Renderer *pRenderer, std::shared_ptr<GameState> gameState);
        virtual ~StarMapScene();

        void render();
        void update();
        void handleEvents(core::Input *pInput);

    private:
        void renderUI();
        std::shared_ptr<GameState> gameState;

        graphics::Text starText;
        graphics::Text uiText;
        graphics::Text glyphText;
        std::shared_ptr<UI::IconButton> cashButton;
        std::shared_ptr<UI::IconButton> researchButton;

        graphics::Texture background;
        std::shared_ptr<graphics::Texture> uiTexture;
        graphics::Texture planetSurfaceTexture;
        std::map<StarType, graphics::Texture *> starTextures;
        std::map<PlanetType, std::shared_ptr<graphics::Texture>> planetTextures;
        graphics::Rect viewPort;
        graphics::Rect defaultViewPort;
        graphics::Rect mainViewPort;
        std::shared_ptr<UI::Container> container;
        std::shared_ptr<UI::Button> playButton;
        std::shared_ptr<UI::Button> pauseButton;
        std::shared_ptr<UI::Button> doubleSpeed;
        windows::PlanetWindow planetWindow;
        ResearchWindow researchWindow;
        std::unique_ptr<TimeThread> timeThread;
        std::shared_ptr<Fleet> selectedFleet;
        std::map<std::string, std::shared_ptr<windows::FleetWindow>> fleetWindows;
        utils::Vector2 targetFleetVec;

        int mouseSpeedX, mouseSpeedY;
        MovementDirection direction;
    };

} /* namespace scenes */

#endif /* SCENES_STARMAPSCENE_H_ */
