#ifndef SHIPLISTVIEW_H
#define SHIPLISTVIEW_H

#include <engine/ui/Object.h>
#include "../fleet.h"

namespace UI {

class ShipListView : public UI::Object
{
public:
    ShipListView(const std::shared_ptr<Fleet>& fleet, UI::Object* parent);
    virtual ~ShipListView();
    virtual void render(core::Renderer* pRender);
    virtual void handleEvents(core::Input* pInput);



    std::shared_ptr<Ship> getSelectedShip() const;

private:
    std::shared_ptr<Fleet> fleet;
    std::shared_ptr<graphics::Texture> background;
    std::shared_ptr<Ship> selectedShip;
};

}

#endif // SHIPLISTVIEW_H
