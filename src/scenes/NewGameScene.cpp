#include "NewGameScene.h"
#include <engine/core/SceneManager.h>
#include <engine/graphics/TextureManager.h>
#include <engine/ui/TextItem.h>
#include <engine/ui/ComboBox.h>
#include "../translate.h"
#include <string>
#include "StarMapScene.h"
#include "../services/buildingservice.h"
#include "../services/researchservice.h"
#include "../WorldGenerator.h"

namespace scenes
{
    NewGameScene::NewGameScene(core::Renderer *pRenderer, core::SceneManager *pSceneManager) : core::Scene(pRenderer), sceneManager(pSceneManager)
    {

        const auto &viewPort = pRenderer->getViewPort();
        container = std::make_shared<UI::Container>();

        auto startButton = std::make_shared<UI::Button>();
        startButton->setPos(viewPort.width - 150, viewPort.height - 50);
        startButton->setFont("fonts/Audiowide-Regular.ttf", 14);
        startButton->setLabel(_("Start Game"));

        startButton->connect(UI::Button::buttonClickCallback(), [&]() { startGame(); });
        container->addObject(startButton);

        auto backButton = std::make_shared<UI::Button>();
        backButton->setPos(viewPort.width - 250, viewPort.height - 50);
        backButton->setFont("fonts/Audiowide-Regular.ttf", 14);
        backButton->setLabel(_("Back"));

        container->addObject(backButton);

        //add comboboxes and textfields
        int yOffset = 30;
        int y = 300;

        auto nameLabel = std::make_shared<UI::Label>(nullptr);
        nameLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        nameLabel->setText(_("Name: "));
        nameLabel->setPos(20, y);
        container->addObject(nameLabel);

        auto nameEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        nameEdit->setPos(200, y);
        nameEdit->setFont("fonts/Audiowide-Regular.ttf", 14);
        nameEdit->setText("New Game");

        container->addObject(nameEdit);

        y += yOffset;
        auto seedLabel = std::make_shared<UI::Label>(nullptr);
        seedLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        seedLabel->setText(_("Seed: "));
        seedLabel->setPos(20, y);
        container->addObject(seedLabel);

        auto seedEdit = std::make_shared<UI::TextItem>(nullptr, 200, 25);
        seedEdit->setPos(200, y);
        seedEdit->setFont("fonts/Audiowide-Regular.ttf", 14);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        seedEdit->setText(std::to_string(seed));
        seedEdit->connect("textChanged", [&](std::string data) {
            seed = std::stoul(data);
        });

        container->addObject(seedEdit);

        y += yOffset;
        auto starsystemLabel = std::make_shared<UI::Label>(nullptr);
        starsystemLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        starsystemLabel->setText(_("Starsystem: "));
        starsystemLabel->setPos(20, y);
        container->addObject(starsystemLabel);

        auto starsystemCombobox = std::make_shared<UI::ComboBox<std::string>>();
        starsystemCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        starsystemCombobox->addElement("Random");
        starsystemCombobox->addElement("Spiral");
        starsystemCombobox->addElement("???");

        starsystemCombobox->setPos(200, y);
        starsystemCombobox->setWidth(200);
        starsystemCombobox->setElementFunction([](std::string val) {
            return val;
        });
        container->addObject(starsystemCombobox);

        y += yOffset;
        auto numberOfPlayersLabel = std::make_shared<UI::Label>(nullptr);
        numberOfPlayersLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        numberOfPlayersLabel->setText(_("Number of Players: "));
        numberOfPlayersLabel->setPos(20, y);
        container->addObject(numberOfPlayersLabel);

        auto numberOfPlayersCombobox = std::make_shared<UI::ComboBox<int>>();
        numberOfPlayersCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        numberOfPlayersCombobox->addElement(2);
        numberOfPlayersCombobox->addElement(4);
        numberOfPlayersCombobox->addElement(8);
        numberOfPlayersCombobox->addElement(16);
        numberOfPlayersCombobox->addElement(32);
        numberOfPlayersCombobox->setPos(200, y);
        numberOfPlayersCombobox->setSelectionByText(8);
        numberOfPlayersCombobox->setWidth(200);
        numberOfPlayersCombobox->setElementFunction([](int val) {
            return std::to_string(val);
        });
        container->addObject(numberOfPlayersCombobox);

        y += yOffset;
        auto systemSizeLabel = std::make_shared<UI::Label>(nullptr);
        systemSizeLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        systemSizeLabel->setText(_("Size of the System: "));
        systemSizeLabel->setPos(20, y);
        container->addObject(systemSizeLabel);

        auto systemSizeCombobox = std::make_shared<UI::ComboBox<std::string>>();
        systemSizeCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        systemSizeCombobox->addElement("Small");
        systemSizeCombobox->addElement("Medium");
        systemSizeCombobox->addElement("Big");
        systemSizeCombobox->addElement("Huge");
        systemSizeCombobox->setSelectionByText("Medium");

        systemSizeCombobox->setPos(200, y);
        systemSizeCombobox->setWidth(200);
        systemSizeCombobox->setElementFunction([](std::string val) {
            return val;
        });
        container->addObject(systemSizeCombobox);

        y += yOffset;
        auto difficultyLabel = std::make_shared<UI::Label>(nullptr);
        difficultyLabel->setFont("fonts/Audiowide-Regular.ttf", 14);
        difficultyLabel->setText(_("Difficultiy: "));
        difficultyLabel->setPos(20, y);
        container->addObject(difficultyLabel);

        auto difficultyCombobox = std::make_shared<UI::ComboBox<std::string>>();
        difficultyCombobox->setFont("fonts/Audiowide-Regular.ttf", 14);
        difficultyCombobox->addElement("Easy");
        difficultyCombobox->addElement("Normal");
        difficultyCombobox->addElement("Hard");
        difficultyCombobox->addElement("Impossible");
        difficultyCombobox->setSelectionByText("Normal");

        difficultyCombobox->setPos(200, y);
        difficultyCombobox->setWidth(200);
        difficultyCombobox->setElementFunction([](std::string val) {
            return val;
        });
        container->addObject(difficultyCombobox);

        bgTexture = graphics::TextureManager::Instance().loadTexture("images/title_background.png");
    }

    void NewGameScene::render()
    {
        bgTexture->renderResized(renderer, 0, 0,
                                 renderer->getMainCamera()->getWidth(),
                                 renderer->getMainCamera()->getHeight());

        container->render(renderer);
    }

    void NewGameScene::update()
    {
    }

    void NewGameScene::handleEvents(core::Input *pInput)
    {
        container->handleEvents(pInput);
    }

    void NewGameScene::startGame()
    {
        std::cout << "start game" << std::endl;
        WorldGenerator gen(seed);
        std::vector<std::shared_ptr<Player>> players;
        std::vector<std::shared_ptr<Building>> buildings = BuildingService::Instance().getData();
        Race human(RaceType::Human, _("Human"), "Sol");
        human.setAvailableBuildings(buildings);
        human.setAvailableResearch(ResearchService::Instance().getData());
        Race psilons(RaceType::Psilons, _("Psilon"), "Mentar");
        psilons.setAvailableBuildings(buildings);
        psilons.setAvailableResearch(ResearchService::Instance().getData());
        SDL_Color blue{0, 0, 200, 0};
        SDL_Color green{0, 255, 0, 0};

        players.push_back(std::make_shared<Player>(_("Human"), human, blue));
        players.push_back(std::make_shared<Player>(_("psilons"), psilons, green));

        std::vector<std::shared_ptr<Star>> stars = gen.generateStarsystem(20, players);

        for (auto &star : stars)
        {
            std::cout << "Starname: " << star->getName() << std::endl;
            std::cout << "Position: (" << star->getPosition().getX() << ","
                      << star->getPosition().getY() << ")" << std::endl;
            for (auto &planet : star->getPlanets())
            {
                std::cout << "\tPlanetname: " << planet->getName() << std::endl;
                std::cout << "\tPlanet Size: " << planet->getSize() << std::endl;
            }
        }

        auto starMapScene = std::make_shared<StarMapScene>(renderer, stars, players[0]);
        sceneManager->addScene("map", starMapScene);
        sceneManager->setCurrentScene("map");
    }
} // namespace scenes