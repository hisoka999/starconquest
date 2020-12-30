/*
 * Planet.cpp
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#include "Planet.h"
#include "messagetypes.h"
#include "translate.h"
#include <engine/core/MessageSystem.h>
#include <engine/graphics/TextureManager.h>
#include <iostream>
#include <memory>
#include <stdexcept>

Planet::Planet(const std::string& Name, const PlanetType Type,
    unsigned int Size, int Angle)
    : name(Name)
    , type(Type)
    , size(Size)
    , player(nullptr)
    , angle(Angle)
{
    population = 0;
    updateMaxPopulation();
    selectedRow = -1;
    selectedColumn = -1;
    fields.fill(FieldType::Grass);
    gridWidth = 40;
    gridHeight = 50;

    updateSurface = false;
    debugText = graphics::TextureManager::Instance().loadFont("fonts/Audiowide-Regular.ttf", 8);

    if (name == "Mentar 1")
        std::cerr << "Mentar 1 was created" << std::endl;
}

Planet::Planet(const Planet& orig)
{
    name = orig.name;
    type = orig.type;
    size = orig.size;
    player = orig.player;
    angle = orig.angle;
    population = orig.population;
    maxPopulation = orig.maxPopulation;
    fields = orig.fields;
    updateSurface = orig.updateSurface;
    gridHeight = orig.gridHeight;
    gridWidth = orig.gridWidth;
    selectedRow = -1;
    selectedColumn = -1;
    std::cout << "copy of planet was created" << std::endl;
}

void Planet::updateMaxPopulation()
{
    switch (type) {
    case PlanetType::Terran:
        maxPopulation = size * 2;
        break;
    case PlanetType::Gaia:
        maxPopulation = size * 3;
        break;
    case PlanetType::Radiated:
        maxPopulation = size;
        break;
    default:
        maxPopulation = size * 1.5;
        break;
    }
}

std::string Planet::getName()
{
    return name;
}
PlanetType Planet::getType()
{
    return type;
}

std::string Planet::getTranslatedType()
{
    switch (type) {
    case PlanetType::Barren:
        return _("Barren");
    case PlanetType::Gaia:
        return _("Gaia");
    case PlanetType::Radiated:
        return _("Radiated");
    case PlanetType::Swamp:
        return _("Swamp");
    case PlanetType::Terran:
        return _("Terran");
    case PlanetType::Wasteland:
        return _("Wasteland");
    default:
        return "unkown";
    }
}

unsigned int Planet::getSize()
{
    return size;
}
unsigned int Planet::getPopulation()
{
    return population;
}
unsigned int Planet::getMaxPopulation()
{
    return maxPopulation;
}
Planet::~Planet()
{
}
void Planet::colonize(const std::shared_ptr<Player>& player)
{
    setPlayer(player);
    initPopulation(10);
    switch (type) {
    case PlanetType::Terran:
        break;
    case PlanetType::Gaia:
        break;
    case PlanetType::Radiated: //fallstrough
    case PlanetType::Barren:
        break;

    default:
        break;
    }
}

int Planet::caclulateFoodPerField(size_t field)
{
    int food = 0;
    switch (fields[field]) {
    case FieldType::Dirt:
        food = 1;
        break;
    case FieldType::Grass:
        food = 2;
        break;
    case FieldType::Waste:
        food = 0;
        break;
    case FieldType::Water:
        food = 1;
        break;
    case FieldType::Forrest:
        food = 2;
        break;
    case FieldType::Mounten:
        food = 0;
        break;
    }
    return food;
}

int Planet::caclulateRessourcePerField(size_t field)
{
    int ressource = 0;
    switch (fields[field]) {
    case FieldType::Dirt:
        ressource = 1;
        break;
    case FieldType::Grass:
        ressource = 0;
        break;
    case FieldType::Waste:
        ressource = 1;
        break;
    case FieldType::Water:
        ressource = 0;
        break;
    case FieldType::Forrest:
        ressource = 1;
        break;
    case FieldType::Mounten:
        ressource = 2;
        break;
    }
    return ressource;
}

int Planet::getSelectedColumn() const
{
    return selectedColumn;
}

int Planet::getSelectedRow() const
{
    return selectedRow;
}

double Planet::populationGrowth()
{
    int food = caclulateFood();
    double consumption = population / 4.0;
    return ((food - consumption) / food);
}

void Planet::updatePopulation()
{
    population += populationGrowth();
    if (population >= maxPopulation)
        population = maxPopulation;
}

int Planet::caclulateFood()
{
    int baseFood = 4;
    int result = baseFood;

    for (size_t pos = 0; pos < FIELDS; ++pos) {
        if (buildings.at(pos) != nullptr) {
            auto& building = buildings[pos];
            result += building->getModifier(ModifierType::Food) + caclulateFoodPerField(pos);
        }
    }
    return result;
}
int Planet::calculateResources()
{
    int baseRessource = 10;
    int result = baseRessource;

    for (size_t pos = 0; pos < FIELDS; ++pos) {
        if (buildings.at(pos) != nullptr) {
            auto& building = buildings[pos];
            result += building->getModifier(ModifierType::Ressource) + caclulateRessourcePerField(pos);
        }
    }
    return result;
}
int Planet::calculateIncome()
{
    int result = population * 0.5;

    for (size_t pos = 0; pos < FIELDS; ++pos) {
        if (buildings.at(pos) != nullptr) {
            auto& building = buildings[pos];
            result += building->getModifier(ModifierType::Money);
        }
    }
    return result;
}
int Planet::caluclateResearch()
{
    int baseRessource = 1;
    int result = baseRessource;

    for (size_t pos = 0; pos < FIELDS; ++pos) {
        if (buildings.at(pos) != nullptr) {
            auto& building = buildings[pos];
            result += building->getModifier(ModifierType::Research);
        }
    }
    return result;
}

void Planet::initPopulation(int population)
{
    if (this->population != 0)
        throw std::runtime_error(
            "you are not allowed to change a planets population");
    this->population = population;
}

void Planet::setPlayer(std::shared_ptr<Player> player)
{
    this->player = player;
}

std::shared_ptr<Player> Planet::getPlayer() const
{
    return player;
}
int Planet::getAngle() const
{
    return angle;
}

void Planet::addBuildingToQueue(int position, std::shared_ptr<BuildableObject> obj)
{
    BuildQueueElement element;
    element.object = obj;
    element.position = position;
    element.resourcesLeft = obj->getRessources();
    buildqueue.push_back(element);
}

int Planet::getGridWidth()
{
    return gridWidth;
}
int Planet::getGridHeight()
{
    return gridHeight;
}
FieldType Planet::getField(int row, int column)
{
    return fields[(row * FIELDS_PER_ROW) + column];
}
void Planet::setSelectedField(int selectedRow, int selectedColumn)
{
    this->selectedRow = selectedRow;
    this->selectedColumn = selectedColumn;

    updateSurface = true;
}

void Planet::generateSurface(long seed)
{
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> chanceGen(0, 100);
    int grass = 20;
    int water = 10;
    int mounten = 30;
    int waste = 40;
    int dirt = 0;
    int forest = 0;
    for (size_t i = 0; i < FIELDS; ++i) {
        grass = 0;
        water = 0;
        mounten = 0;
        waste = 0;
        dirt = 0;
        forest = 0;
        switch (type) {
        case PlanetType::Barren:
            grass = 10;
            water = 10;
            mounten = 30;
            waste = 50;
            break;
        case PlanetType::Wasteland:
            grass = 0;
            water = 0;
            mounten = 30;
            waste = 70;
            break;
        case PlanetType::Terran:
            grass = 20;
            water = 20;
            mounten = 20;
            forest = 40;
            break;
        case PlanetType::Swamp:
            grass = 30;
            forest = 50;
            water = 30;
            mounten = 20;
            break;
        default:
            grass = 00;
            dirt = 20;
            waste = 50;
            mounten = 30;
            break;
        }
        int chance = chanceGen(generator);
        int base = 0;
        if (chance < grass && chance > base) {
            fields[i] = FieldType::Grass;
        }
        base += grass;
        if (chance < base + water && chance > base) {
            fields[i] = FieldType::Water;
        }
        base += water;
        if (chance < base + mounten && chance > base) {
            fields[i] = FieldType::Mounten;
        }
        base += mounten;
        if (chance < base + waste && chance > base) {
            fields[i] = FieldType::Waste;
        }
        base += waste;
        if (chance < base + dirt && chance > base) {
            fields[i] = FieldType::Dirt;
        }
        base += dirt;
        if (chance < base + forest && chance > base) {
            fields[i] = FieldType::Forrest;
        }
        base += forest;
    }
    updateSurface = true;
}
int Planet::getSelectedField()
{
    return selectedColumn + (selectedRow * FIELDS_PER_ROW);
}
std::vector<std::shared_ptr<BuildableObject>> Planet::getBuildableBuildings(
    int row, int column)
{
    std::vector<std::shared_ptr<BuildableObject>> buildings;
    auto playerBuildings = player->getRace().getAvailableBuildings();
    for (auto& building : playerBuildings) {

        bool buildable = true;

        auto resList = player->getRace().getAvailableResearch();
        for (auto& res : resList) {
            if (res->canEnableObject(building->getName())) {
                if (!res->getResearched()) {
                    buildable = false;
                }
            }
        }
        if (buildable) {
            buildings.push_back(building);
        }
    }

    //filter all unique buildings

    //filter buildings by type

    return buildings;
}

void Planet::findSrcRectFloor(graphics::Rect* target, FieldType fieldType)
{
    target->width = 120;
    target->height = 140;
    switch (fieldType) {
    case FieldType::Forrest:
        target->x = 488;
        target->y = 1136;
        break;
    case FieldType::Grass:
        target->x = 610;
        target->y = 142;
        break;
    case FieldType::Mounten:
        if (type == PlanetType::Radiated
            || type == PlanetType::Wasteland) {
            target->x = 122;
            target->y = 994;
        } else {
            target->x = 488;
            target->y = 710;
        }

        break;
    case FieldType::Water:
        target->x = 1465;
        target->y = 0;
        break;
    case FieldType::Waste:
        target->x = 244;
        target->y = 426;
        break;
    case FieldType::Dirt:
        target->x = 366;
        target->y = 1136;
        break;
    default:
        break;
    }
}

void Planet::renderPlanetSurface(core::Renderer* pRenderer,
    std::shared_ptr<graphics::Texture> pTexture, graphics::Texture* pTargetSurface)
{
    if (pTargetSurface == nullptr)
        return;

    SDL_SetTextureBlendMode(pTargetSurface->getSDLTexture(),
        SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(pRenderer->getRenderer(), 0, 0, 0, 0);

    int column = 0;
    int row = 0;
    int width = gridWidth;
    int height = gridHeight;
    pRenderer->setRenderTarget(pTargetSurface->getSDLTexture());
    pRenderer->clear();
    graphics::Rect srcRect, destRect;
    srcRect = { 0, 0, 0, 0 };
    for (size_t i = 0; i < FIELDS; ++i, ++column) {
        if (column >= FIELDS_PER_ROW) {
            column = 0;
            row++;
        }
        findSrcRectFloor(&srcRect, fields[i]);
        //render floor
        destRect.x = (column * width) + ((row % 2 == 0) ? 0 : width / 2);

        destRect.y = row * (height - (35.0 / 140.0 * height));
        destRect.height = height;
        destRect.width = width;
        pTexture->render(pRenderer, srcRect, destRect);
        SDL_Color white = { 255, 255, 255, 255 };
        debugText->render(pRenderer, std::to_string(row) + "/" + std::to_string(column), white, destRect.x, destRect.y);

        //render building
    }
    row = 0;
    column = 0;
    for (size_t i = 0; i < FIELDS; ++i, ++column) {
        if (column >= FIELDS_PER_ROW) {
            column = 0;
            row++;
        }
        destRect.x = (column * width) + ((row % 2 == 0) ? 0 : width / 2);

        destRect.y = row * (height - (35.0 / 140.0 * height));
        if (buildings.at(i) != nullptr) {
            buildings[i]->getTexture()->renderResized(pRenderer, destRect.x, destRect.y, width, height);
        }
    }
    if (selectedRow >= 0 && selectedColumn >= 0) {
        srcRect.x = 124;
        srcRect.y = 568;
        destRect.x = (selectedColumn * width)
            + ((selectedRow % 2 == 0) ? 0 : width / 2);
        destRect.y = selectedRow * (height - (35.0 / 140.0 * height));
        pTexture->render(pRenderer, srcRect, destRect);
    }

    pRenderer->setRenderTarget(nullptr);
    pRenderer->clear();
    updateSurface = false;
}
void Planet::updateBuildQueue()
{
    int resources = calculateResources();
    if (resources <= 0 || buildqueue.empty())
        return;

    auto& element = buildqueue.front();
    element.resourcesLeft -= resources;
    if (element.resourcesLeft <= 0) {
        int diff = element.resourcesLeft;

        const std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(element.object);
        if (b != nullptr) {
            buildings[element.position] = b;
        } else {
            const std::shared_ptr<Ship> ship = std::dynamic_pointer_cast<Ship>(element.object);
            auto& sys = core::MessageSystem<MessageTypes>::get();

            ShipBuildData data;
            data.ship = ship;
            data.planet = this;
            auto msg = std::make_shared<core::Message<MessageTypes, ShipBuildData>>(MessageTypes::ShipHasBuild, data);
            sys.sendMessage(msg);
        }

        buildqueue.erase(buildqueue.begin());
    }
}

std::vector<BuildQueueElement> Planet::getQueuedObjects()
{
    return buildqueue;
}
bool Planet::hasFieldBuilding(int row, int column)
{
    size_t field = row + (column * FIELDS_PER_ROW);
    return buildings[field] != nullptr;
}
bool Planet::hasBuildingOfName(const std::string& buildingName)
{
    for (auto& building : buildings) {
        if (building == nullptr)
            continue;
        if (building->getName() == buildingName)
            return true;
    }
    return false;
}
