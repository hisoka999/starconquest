#include "shipyardtab.h"
#include "../services/shipservice.h"
#include "../translate.h"
#include "../ui/BuildableObjectView.h"
namespace UI {

ShipyardTab::ShipyardTab(Object* parent, const std::string& title, const std::shared_ptr<Planet>& planet)
    : UI::Tab(parent, title)
    , planet(planet)
{

    buildArea = std::make_shared<UI::ScrollArea>(200, 300, this);
    buildArea->setPos(10, 10);
    buildButton = std::make_shared<UI::Button>(this);
    buildButton->setLabel(_("Build"));
    buildButton->disable();
    buildButton->setPos(110, 320);
    auto ships = ShipService::Instance().getData();

    buildButton->connect(UI::Button::buttonClickCallback(), [&]() {
        auto list = ShipService::Instance().getData();
        planet->addBuildingToQueue(-1, list[selectedShip]);
    });

    buildArea->connect("selectItem", [&](int idx) {
        buildButton->enable();
        selectedShip = idx;
    });

    addObject(buildArea);
    addObject(buildButton);

    int i = 0;
    for (auto& ship : ships) {
        std::shared_ptr<BuildableObjectView> shipView = std::make_shared<BuildableObjectView>(ship, buildArea.get());
        shipView->update(planet->calculateResources());
        buildArea->addObject(shipView);
        shipView->setY(i * 50);
        i++;
    }
}

}
