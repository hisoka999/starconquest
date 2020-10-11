#include "shipyardtab.h"
#include "../translate.h"

namespace UI {

ShipyardTab::ShipyardTab(Object* parent, std::string title)
    : UI::Tab(parent, title)
{

    buildArea = std::make_shared<UI::ScrollArea>(200, 400, this);
    buildArea->setPos(10, 10);
    buildButton = std::make_shared<UI::Button>(this);
    buildButton->setLabel(_("Build"));
    buildButton->disable();
    buildButton->setPos(110, 420);

    buildArea->connect("selectItem", [&](int idx) {
        buildButton->enable();
    });

    addObject(buildArea);
    addObject(buildButton);
}

}
