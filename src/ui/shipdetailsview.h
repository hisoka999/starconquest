#ifndef SHIPDETAILSVIEW_H
#define SHIPDETAILSVIEW_H

#include <engine/ui/Container.h>
#include <engine/ui/Label.h>
#include "../Ship.h"
namespace UI {

class ShipDetailsView : public UI::Container,public UI::Object
{
public:
    ShipDetailsView(UI::Object* parent = nullptr);
    virtual void render(core::Renderer* pRender);
    virtual void handleEvents(core::Input* pInput);
    void setShip(const std::shared_ptr<Ship> &value);

private:
    std::shared_ptr<Ship> ship;
    //ui elements
    std::shared_ptr<UI::Label> nameLabel;
    std::shared_ptr<UI::Label> costsLabel;
    std::shared_ptr<UI::Label> typeLabel;
    std::shared_ptr<UI::Label> descriptionLabel;

    std::shared_ptr<UI::Label> nameText;
    std::shared_ptr<UI::Label> costsText;
    std::shared_ptr<UI::Label> typeText;
    std::vector<std::shared_ptr<UI::Label>> attributeLabels;
};
}

#endif // SHIPDETAILSVIEW_H
