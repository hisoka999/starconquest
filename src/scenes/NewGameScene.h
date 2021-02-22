#pragma once
#include <engine/core/Scene.h>
#include "../constants.h"
#include "../Race.h"
namespace core
{
    class SceneManager;
};
namespace UI
{
    class ScrollArea;
};

namespace scenes
{

    class NewGameScene : public core::Scene
    {
    public:
        NewGameScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager);

        void render();
        void update();
        void handleEvents(core::Input *pInput);

    private:
        void startGame();

        core::SceneManager *sceneManager;
        std::shared_ptr<graphics::Texture> bgTexture;
        std::shared_ptr<UI::Container> container;
        std::shared_ptr<UI::Container> buttonList;
        std::shared_ptr<UI::ScrollArea> scrollArea;
        unsigned seed;
        WorldSize worldSize;
        int numberOfPlayers;
        std::vector<std::shared_ptr<Race>> avaiableRaces;
        std::shared_ptr<Race> selectedRace;
    };
}; // namespace scenes
