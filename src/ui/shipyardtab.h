#ifndef SHIPYARDTAB_H
#define SHIPYARDTAB_H

#include "../Star.h"
#include "BuildableObjectView.h"
#include <engine/ui/Button.h>
#include <engine/ui/Tab.h>
#include <engine/ui/scrollarea.h>
namespace UI {

class ShipyardTab : public UI::Tab {
public:
    ShipyardTab(Object* parent, const std::string& title, const std::shared_ptr<Planet>& planet);

private:
    std::shared_ptr<UI::ScrollArea> buildArea;
    std::shared_ptr<UI::Button> buildButton;
    int selectedShip = -1;

    //TODO ship details
    std::shared_ptr<Planet> planet;
};
}

#endif // SHIPYARDTAB_H
