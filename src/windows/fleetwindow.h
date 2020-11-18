#ifndef FLEETWINDOW_H
#define FLEETWINDOW_H

#include "../fleet.h"
#include "../ui/shipdetailsview.h"
#include "../ui/shiplistview.h"
#include <engine/ui/Window.h>

namespace windows {

class FleetWindow : public UI::Window {
public:
    FleetWindow();

    void setFleet(const std::shared_ptr<Fleet>& fleet);

private:
    std::shared_ptr<Fleet> fleet;
    std::shared_ptr<UI::ShipListView> shipListView;
    std::shared_ptr<UI::ShipDetailsView> shipDetailsView;
};

}
#endif // FLEETWINDOW_H
