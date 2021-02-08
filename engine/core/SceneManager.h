/*
 * SceneManager.h
 *
 *  Created on: 26.04.2016
 *      Author: stefan
 */

#ifndef CORE_SCENEMANAGER_H_
#define CORE_SCENEMANAGER_H_

#include "engine/core/input.h"
#include <map>
#include <string>
#include <memory>
#include <mutex>

namespace core
{

    class Scene;

    class SceneManager
    {
    public:
        static SceneManager &Instance()
        {
            std::call_once(onceFlag, [] {
                initSingleton();
            });

            return *(instance);
        }

        SceneManager();
        virtual ~SceneManager();

        void render();
        void handleEvents(core::Input *pInput);

        void addScene(std::string name, const std::shared_ptr<Scene> &scene);
        void setCurrentScene(std::string name);
        const std::shared_ptr<Scene> &getScene(std::string name);
        void update();
        const std::shared_ptr<Scene> &getCurrentScene();

    private:
        std::map<std::string, std::shared_ptr<Scene>> scenes;
        std::string currentScene;

        static SceneManager *instance;
        static std::once_flag onceFlag;

        static void initSingleton()
        {
            instance = new SceneManager();
        }
    };

} /* namespace core */

#endif /* CORE_SCENEMANAGER_H_ */
