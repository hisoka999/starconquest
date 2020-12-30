/*
 * PlanetWindow.cpp
 *
 *  Created on: 29.05.2016
 *      Author: stefan
 */

#include "PlanetWindow.h"
#include "../translate.h"
#include "../ui/buildqueueobjectview.h"
#include <engine/graphics/TextureManager.h>

#include <functional>

#include "../messagetypes.h"
#include <engine/core/MessageSystem.h>
#include <engine/utils/os.h>

namespace windows {

PlanetWindow::PlanetWindow()
    : UI::Window(0, 0, 900, 450)
{
    uiText = graphics::TextureManager::Instance().loadFont("fonts/Audiowide-Regular.ttf", 12);
    uiIconText = graphics::TextureManager::Instance().loadFont("fonts/fa-solid-900.ttf", 20);

    setFont(uiText.get());

    bar = std::make_shared<UI::TabBar>(this);
    bar->setWidth(550);
    bar->setHeight(405);
    addObject(bar);
    setTitle(_("Planetview"));
    mainTab = std::make_shared<UI::Tab>(bar.get(), _("Overview"));
    buildTab = std::make_shared<UI::Tab>(bar.get(), _("Buildings"));
    shipyardTab = nullptr;
    bar->addTab(mainTab);
    bar->addTab(buildTab);

    colonizeButton = std::make_shared<UI::Button>(mainTab.get());
    colonizeButton->setLabel(_("Colonize"));
    colonizeButton->setPos(100, 180);
    colonizeButton->connect(UI::Button::buttonClickCallback(), [=]() {
        planet->colonize(gameState->getHumanPlayer());
        colonizeButton->disable();

        auto fleets = gameState->findFleetsInPlanetDistance(star, planet);

        for (auto& fleet : fleets) {
            if (fleet->getOwner() == gameState->getHumanPlayer().get()) {
                for (auto& ship : fleet->getShips()) {
                    if (ship->getShipType() == ShipType::ColonyShip) {
                        fleet->destroyShip(ship);
                        if (fleet->getShips().empty()) {
                            gameState->removeFleet(fleet);
                        }
                        break;
                    }
                }
            }
        }
    });
    mainTab->addObject(colonizeButton);
    nameLabel = std::make_shared<UI::Label>(_("Planet: "), mainTab.get());
    nameLabel->setPos(10, 50);
    mainTab->addObject(nameLabel);
    typeLabel = std::make_shared<UI::Label>(_("Type: "), mainTab.get());
    typeLabel->setPos(10, 80);
    mainTab->addObject(typeLabel);
    planetOwnerLabel = std::make_shared<UI::Label>(_("Owner: "), mainTab.get());
    planetOwnerLabel->setPos(10, 110);
    mainTab->addObject(planetOwnerLabel);

    mainTab->setTitle(_("Planet Overview"));

    populationIconLabel = std::make_shared<UI::Label>(mainTab.get());
    populationIconLabel->setFont(uiIconText.get());
    populationIconLabel->setText("\uf0c0");
    populationIconLabel->setPos(10, 0);
    populationLabel = std::make_shared<UI::Label>(mainTab.get());
    populationLabel->setFont(uiText.get());
    populationLabel->setText("0");
    populationLabel->setPos(40, 5);

    foodIconLabel = std::make_shared<UI::Label>(mainTab.get());
    foodIconLabel->setFont(uiIconText.get());
    foodIconLabel->setText("\uf4d8");
    foodIconLabel->setPos(120, 0);
    foodLabel = std::make_shared<UI::Label>(mainTab.get());
    foodLabel->setFont(uiText.get());
    foodLabel->setText("0");
    foodLabel->setPos(150, 5);

    ressourceIconLabel = std::make_shared<UI::Label>(mainTab.get());
    ressourceIconLabel->setFont(uiIconText.get());
    ressourceIconLabel->setText("\uf013");
    ressourceIconLabel->setPos(230, 0);
    ressourceLabel = std::make_shared<UI::Label>(mainTab.get());
    ressourceLabel->setFont(uiText.get());
    ressourceLabel->setText("0");
    ressourceLabel->setPos(260, 5);

    planetIcon = std::make_shared<UI::ImageButton>(mainTab.get(), 200, 200, 0, 0, true);
    planetIcon->setPos(250, 200);
    mainTab->addObject(planetIcon);

    mainTab->addObject(foodIconLabel);
    mainTab->addObject(foodLabel);
    mainTab->addObject(populationIconLabel);
    mainTab->addObject(populationLabel);
    mainTab->addObject(ressourceLabel);
    mainTab->addObject(ressourceIconLabel);

    planetSurface = std::make_shared<UI::ImageButton>(buildTab.get(), 440, 320, 0, 0);
    planetSurface->setPos(10, 10);

    buildWindow = new BuildWindow(nullptr);
    buildWindow->setPos(300, 300); //TODO
    buildWindow->setFont(uiText.get());
    buildQueueArea = std::make_shared<UI::ScrollArea>(200, 350, this);
    buildQueueArea->setPos(670, 10);
    this->addObject(buildQueueArea);
    buildButton = std::make_shared<UI::Button>(buildTab.get());
    buildTab->addObject(buildButton);
    buildButton->setLabel(_("Build"));
    buildButton->setPos(350, 250);
    buildButton->connect(UI::Button::buttonClickCallback(), [=]() {
        std::cout << "build it" << std::endl;
        buildWindow->setPlayer(planet->getPlayer());
        buildWindow->clearBuildings();
        int ressources = planet->calculateResources();
        for (auto building : planet->getBuildableBuildings(planet->getSelectedRow(), planet->getSelectedColumn())) {
            buildWindow->addBuilding(building, ressources);
        }
        buildWindow->setVisible(true);
    });

    buildWindow->connect("buildingSelected", [&]() {
        std::cout << "building selected: " << buildWindow->getSelectedBuilding()->getName() << std::endl;
        auto building = buildWindow->getSelectedBuilding();

        planet->addBuildingToQueue(planet->getSelectedField(), building);
        updatePlanet = true;
    });

    using namespace std::placeholders;
    //auto func = std::bind(&PlanetWindow::onSurfaceClick, this, _1);
    planetSurface->connect("buttonClickPara", [&](const utils::Vector2& pos) { this->onSurfaceClick(pos); });
    buildTab->addObject(planetSurface);
    updatePlanet = false;
    surfaceTexture = nullptr;
    surfaceTiles = nullptr;

    auto& msgSystem = core::MessageSystem<MessageTypes>::get();
    msgSystem.registerForType(MessageTypes::NewMonth, [&](int val) {
        updatePlanet = true;
        updateData();
    });

    planetTextures[PlanetType::Barren] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_23.png");

    planetTextures[PlanetType::Gaia] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_31.png");

    planetTextures[PlanetType::Radiated] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_29.png");

    planetTextures[PlanetType::Swamp] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_23.png");

    planetTextures[PlanetType::Terran] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_27.png");

    planetTextures[PlanetType::Wasteland] = graphics::TextureManager::Instance().loadTexture("images/planets/planet_30.png");
}
void PlanetWindow::onOpen()
{
    updateData();
}

void PlanetWindow::updateData()
{
    if (planet == nullptr)
        return;
    int growth = planet->populationGrowth() * 100.0;
    if (planet->getPlayer() != nullptr) {
        populationLabel->setTextF("%d [%+d]", planet->getPopulation(), growth);

        foodLabel->setTextF("%+d", planet->caclulateFood());
        ressourceLabel->setTextF("%+d", planet->calculateResources());

        planetOwnerLabel->setTextF(_("Owner: %s"), planet->getPlayer()->getName());
    } else {
        populationLabel->setTextF("0");

        foodLabel->setTextF("0");
        ressourceLabel->setTextF("0");

        planetOwnerLabel->setTextF(_("Owner: "));
    }
    planetIcon->setImage(planetTextures[planet->getType()]);

    //has planet shipyard
    if (planet->hasBuildingOfName("Shipyard") && shipyardTab == nullptr) {
        shipyardTab = std::make_shared<UI::ShipyardTab>(bar.get(), _("Shipyard"), planet);
        bar->addTab(shipyardTab);
    } else if (shipyardTab != nullptr && !planet->hasBuildingOfName("Shipyard")) {
        bar->removeTab(shipyardTab);
        shipyardTab = nullptr;
    }
}

void PlanetWindow::onSurfaceClick(const utils::Vector2& pos)
{
    std::cout << "click on surface x = " << pos.getX() << std::endl;
    float gridHeight = planet->getGridHeight();
    float gridWidth = planet->getGridWidth();
    //demo code zum finden des hexagons
    int row = (pos.getY() / (gridHeight - (35.0 / 140.0 * gridHeight)));
    int column;
    bool rowIsOdd = static_cast<int>(row) % 2 == 1;
    // is the row an odd number?
    //destRect.x = (column * width) + ((row % 2 == 0) ? 0 : width / 2);

    //destRect.y = row * (height - (35.0 / 140.0 * height));

    if (rowIsOdd) {
        column = (pos.getX() / gridWidth) - ((gridWidth / 2.f) / gridWidth);
    } else {
        column = pos.getX() / gridWidth;
    }
    std::cout << "row = " << row << " column = " << column << std::endl;
    std::cout << "field type = " << (int)planet->getField(row, column)
              << std::endl;
    planet->setSelectedField(row, column);

    if (planet->hasFieldBuilding(row, column) || planet->getPlayer() != gameState->getHumanPlayer()) {
        buildButton->disable();
    } else {
        buildButton->enable();
    }

    updatePlanet = true;
}

void PlanetWindow::render(core::Renderer* pRender, std::shared_ptr<graphics::Texture> pTexture)
{

    if (updatePlanet && planet != nullptr) {
        if (surfaceTexture != nullptr)
            surfaceTexture = nullptr;

        if (surfaceTiles == nullptr) {
            surfaceTiles = std::make_shared<graphics::Texture>();
            surfaceTiles->loadTexture(pRender,
                utils::os::combine("images", "hexagonAll_sheet.png"));
        }
        int fieldsPerColumn = FIELDS / FIELDS_PER_ROW;
        surfaceTexture = std::make_shared<graphics::Texture>(pRender,
            FIELDS_PER_ROW * planet->getGridWidth(),
            fieldsPerColumn * planet->getGridHeight());

        planet->renderPlanetSurface(pRender, surfaceTiles, surfaceTexture.get());

        planetSurface->setImage(surfaceTexture);
        updatePlanet = false;
        buildQueueArea->clear();
        buildQueueArea->reset();
        int i = 0;
        for (auto& object : planet->getQueuedObjects()) {

            auto view = std::make_shared<UI::BuildQueueObjectView>(&object, buildQueueArea.get());
            view->setY(i * 50);
            view->update(planet->calculateResources());
            buildQueueArea->addObject(view);

            i++;
        }
    }
    UI::Window::render(pRender);
    buildWindow->render(pRender);
}

void PlanetWindow::openForPlanet(const std::shared_ptr<Star>& star, const std::shared_ptr<Planet>& planet)
{
    nameLabel->setText("Planet: " + planet->getName());
    typeLabel->setText("Type: " + planet->getTranslatedType());
    this->planet = planet;
    this->star = star;

    int fieldsPerColumn = FIELDS / FIELDS_PER_ROW;
    planetSurface->setWidth(FIELDS_PER_ROW * planet->getGridWidth());
    planetSurface->setHeight(fieldsPerColumn * planet->getGridHeight());

    auto fleets = gameState->findFleetsInPlanetDistance(star, planet);

    bool colonizeable = false;
    for (auto& fleet : fleets) {
        if (fleet->getOwner() == gameState->getHumanPlayer().get()) {
            for (auto& ship : fleet->getShips()) {
                if (ship->getShipType() == ShipType::ColonyShip) {
                    colonizeable = true;
                    break;
                }
            }
        }
    }

    if (planet->getPlayer() == nullptr && colonizeable) {
        colonizeButton->enable();
    } else {
        colonizeButton->disable();
    }
    updatePlanet = true;

    setVisible(true);
}

PlanetWindow::~PlanetWindow()
{
}
void PlanetWindow::handleEvents(core::Input* pInput)
{
    if (buildWindow->getVisible()) {
        buildWindow->handleEvents(pInput);
    } else {
        UI::Window::handleEvents(pInput);
    }
}

void PlanetWindow::setGameState(const std::shared_ptr<GameState>& gameState)
{
    this->gameState = gameState;
}
} /* namespace windows */
