#include "shipdetailsview.h"
#include "../translate.h"
#include <magic_enum.hpp>

namespace UI {
ShipDetailsView::ShipDetailsView(UI::Object* parent)
    : UI::Container()
    , UI::Object(parent)
{

    nameLabel = std::make_shared<UI::Label>(_("Name: "), this);
    nameLabel->setPos(5, 5);
    addObject(nameLabel);
    costsLabel = std::make_shared<UI::Label>(_("Costs: "), this);
    costsLabel->setPos(5, 25);
    addObject(costsLabel);
    typeLabel = std::make_shared<UI::Label>(_("Type: "), this);
    typeLabel->setPos(5, 45);
    addObject(typeLabel);
    descriptionLabel = std::make_shared<UI::Label>("", this);
    descriptionLabel->setPos(5, 65);
    addObject(descriptionLabel);

    nameText = std::make_shared<UI::Label>("", this);
    nameText->setPos(50, 5);
    addObject(nameText);

    costsText = std::make_shared<UI::Label>("", this);
    costsText->setPos(50, 25);
    addObject(costsText);

    typeText = std::make_shared<UI::Label>("", this);
    typeText->setPos(50, 45);
    addObject(typeText);
}

void ShipDetailsView::render(core::Renderer* pRender)
{
    UI::Container::render(pRender);
}
void ShipDetailsView::handleEvents(core::Input* pInput)
{
    UI::Container::handleEvents(pInput);
}

void ShipDetailsView::setShip(const std::shared_ptr<Ship>& value)
{
    ship = value;

    nameText->setText(ship->getLocalisedName());
    typeText->setText(std::string(magic_enum::enum_name(ship->getShipType())));
    costsText->setTextF("%d", ship->getCostsPerMonth());

    //remove old attibutes
    for (auto& attr : attributeLabels) {
        removeObject(attr);
    }
    int i = 1;
    for (auto& attr : ship->getAttrbuteValues()) {
        auto attrName = std::string(magic_enum::enum_name(attr.first)) + std::string(":");
        auto attrNameLabel = std::make_shared<UI::Label>(attrName, this);
        attrNameLabel->setPos(5, 45 + i * 20);

        auto attrValueLabel = std::make_shared<UI::Label>(std::to_string(attr.second), this);
        attrValueLabel->setPos(50, 45 + i * 20);
        addObject(attrNameLabel);
        addObject(attrValueLabel);
        attributeLabels.push_back(attrNameLabel);
        attributeLabels.push_back(attrValueLabel);
        i++;
    }
}
}
