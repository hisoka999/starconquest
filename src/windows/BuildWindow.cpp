/*
 * BuildWindow.cpp
 *
 *  Created on: 12.03.2018
 *      Author: stefan
 */

#include "BuildWindow.h"
#include "../translate.h"
#include "../ui/BuildableObjectView.h"

namespace windows {

BuildWindow::BuildWindow(std::shared_ptr<Player> p)
    : UI::Window(0, 0, 250, 350)
    , player(p)
    , selection(-1)
{
    scrollArea = std::make_shared<UI::ScrollArea>(200, 300, this);
    setTitle(_("Build"));
    addObject(scrollArea);
    scrollArea->setX(5);
    scrollArea->connect("selectItem", [&](int idx) {
        selection = idx;
        this->setVisible(false);
        this->fireFuncionCall("buildingSelected");
    });
}



void BuildWindow::setPlayer(std::shared_ptr<Player> p)
{
    player = p;
}

std::shared_ptr<BuildableObject> BuildWindow::getSelectedBuilding()
{
    return buildings[selection];
}

BuildWindow::~BuildWindow()
{
    // TODO Auto-generated destructor stub
}

void BuildWindow::clearBuildings()
{
    scrollArea->clear();
    buildings.clear();
}

void BuildWindow::addBuilding(std::shared_ptr<BuildableObject> object, const int planetRessources)
{
    buildings.push_back(object);
    auto obj = std::make_shared<UI::BuildableObjectView>(object,
        scrollArea.get());
    obj->setY((buildings.size() - 1) * 50);
    obj->update(planetRessources);
    scrollArea->addObject(obj);
    scrollArea->reset();
}

} /* namespace windows */
