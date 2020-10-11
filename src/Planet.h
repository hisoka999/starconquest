/*
 * Planet.h
 *
 *  Created on: 18.04.2016
 *      Author: stefan
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "BuildableObject.h"
#include "Building.h"
#include "Player.h"
#include <array>
#include <engine/core/renderer.h>
#include <engine/graphics/text.h>
#include <engine/graphics/texture.h>
#include <engine/utils/vector2.h>
#include <memory>
#include <random>
#include <string>
#include <vector>

#define FIELDS 64
#define FIELDS_PER_ROW 8

enum class PlanetType {
    Barren = 0,
    Wasteland = 1,
    Swamp = 2,
    Radiated = 3,
    Terran = 4,
    Gaia = 5
};

enum class FieldType {
    Grass = 0,
    Mounten = 1,
    Forrest = 2,
    Water = 3,
    Dirt = 4,
    Waste = 5
};

struct BuildQueueElement {
    std::shared_ptr<BuildableObject> object;
    int position;
    int resourcesLeft;
};

class Planet {
public:
    Planet(const std::string& Name, const PlanetType Type, unsigned int Size,
        int Angle);
    virtual ~Planet();
    Planet(const Planet& orig);

    std::string getName();
    PlanetType getType();
    unsigned int getSize();
    unsigned int getPopulation();
    unsigned int getMaxPopulation();
    void colonize(const std::shared_ptr<Player>& player);
    std::shared_ptr<Player> getPlayer() const;
    int getAngle() const;

    void updateMaxPopulation();

    std::string getTranslatedType();

    void addBuildingToQueue(int position, std::shared_ptr<BuildableObject> obj);
    std::vector<std::shared_ptr<BuildableObject>> getQueuedObjects();
    void renderPlanetSurface(core::Renderer* pRenderer,
        std::shared_ptr<graphics::Texture> pTexture, graphics::Texture* pTargetSurface);
    void generateSurface(long seed);
    int getGridWidth();
    int getGridHeight();
    int getSelectedField();
    FieldType getField(int row, int column);
    void setSelectedField(int selectedRow, int selectedColumn);
    std::vector<std::shared_ptr<BuildableObject>> getBuildableBuildings(int row,
        int column);

    int caclulateFood();
    int calculateResources();
    int calculateIncome();
    int caluclateResearch();
    double populationGrowth();
    void updatePopulation();
    void updateBuildQueue();
    bool hasFieldBuilding(int row, int column);
    bool hasBuildingOfName(const std::string& buildingName);

private:
    void initPopulation(int population);
    void setPlayer(std::shared_ptr<Player> player);
    void findSrcRectFloor(graphics::Rect* target, FieldType fieldType);
    int caclulateFoodPerField(size_t field);
    int caclulateRessourcePerField(size_t field);

    std::string name;
    PlanetType type;
    unsigned int size;
    unsigned int maxPopulation;
    double population;
    std::shared_ptr<Player> player;
    int angle;
    std::array<FieldType, FIELDS> fields;
    std::array<std::shared_ptr<Building>, FIELDS> buildings;
    std::vector<BuildQueueElement> buildqueue;
    bool updateSurface;
    int gridWidth;
    int gridHeight;
    int selectedRow, selectedColumn;
    std::shared_ptr<graphics::Text> debugText;
};

#endif /* PLANET_H_ */
