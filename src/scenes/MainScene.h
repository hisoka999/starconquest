/*
 * MainScene.h
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#ifndef SCENES_MAINSCENE_H_
#define SCENES_MAINSCENE_H_

#include "../Building.h"
#include "../windows/SettingsWindow.h"

#include <engine/core/Scene.h>
#include <engine/core/SceneManager.h>
#include <engine/core/renderer.h>
#include <engine/ui/Container.h>
#include <memory>
#include <vector>

namespace scenes
{

    class MainScene : public core::Scene
    {
    public:
        MainScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager, const core::GameWindow *gameWindow);
        virtual ~MainScene();
        void render();
        void handleEvents(core::Input *pInput);
        bool isRunning()
        {
            return running;
        }

    private:
        bool running;
        core::SceneManager *sceneManager;
        std::shared_ptr<UI::Container> container;
        std::shared_ptr<graphics::Texture> bgTexture;
        std::vector<std::shared_ptr<Building>> initBuildings();

        void exitGame();
        void startGame();
        void loadGame();
        SettingsWindow settingsWindow;
    };

} /* namespace scenes */

#endif /* SCENES_MAINSCENE_H_ */
