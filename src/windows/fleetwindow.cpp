#include "fleetwindow.h"
#include <algorithm>

namespace windows {
FleetWindow::FleetWindow()
    : UI::Window(0, 0, 420, 430)
    , fleet(nullptr)
{
    shipDetailsView = std::make_shared<UI::ShipDetailsView>(this);
    addObject(shipDetailsView);
}

void FleetWindow::setFleet(const std::shared_ptr<Fleet>& fleet)
{
    this->fleet = fleet;

    auto fleetName = fleet->getName();

    std::transform(fleetName.begin(), fleetName.end(), fleetName.begin(), ::toupper);

    setTitle(fleetName);
    if (shipListView != nullptr)
        removeObject(shipListView);

    shipListView = std::make_shared<UI::ShipListView>(fleet, this);
    shipListView->setPos(5, 150);
    shipDetailsView->setPos(10 + shipListView->getWidth(), 150);
    shipListView->connect("shipSelected", [=](std::shared_ptr<Ship> ship) {
        shipDetailsView->setShip(ship);
    });
    addObject(shipListView);
}

}
