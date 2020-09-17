/*
 * BuildableObjectView.cpp
 *
 *  Created on: 13.03.2018
 *      Author: stefan
 */

#include "BuildableObjectView.h"
#include <engine/utils/os.h>
namespace UI {

BuildableObjectView::BuildableObjectView(std::shared_ptr<BuildableObject> object,
    Object* parent)
    : UI::Object(parent)
    , UI::Container()
    , object(object)

{
    titleLabel = std::make_shared<UI::Label>(this);
    priceLabel = std::make_shared<UI::Label>(this);
    buildingImage = std::make_shared<UI::ImageButton>(this, 50, 50, 0, 0, true);

    buildUI();
}

void BuildableObjectView::buildUI()
{
    setWidth(200);
    setHeight(50);
    addObject(titleLabel);
    addObject(priceLabel);
    addObject(buildingImage);
    buildingImage->setPos(0, 0);
    buildingImage->setWidth(50);
    buildingImage->setHeight(50);
    SDL_Color black = { 255, 255, 255, 255 };
    SDL_Color red = { 255, 0, 0, 255 };

    titleLabel->setColor(black);
    titleLabel->setPos(55, 5);
    priceLabel->setColor(black);
    priceLabel->setPos(55, 20);

    //update data
    titleLabel->setText(object->getName());
    priceLabel->setText("Price: " + std::to_string(object->getRessources()));
    buildingImage->setImage(object->getTexture());
}

void BuildableObjectView::render(core::Renderer* pRender)
{
    UI::Container::render(pRender);
}
void BuildableObjectView::handleEvents(core::Input* pInput)
{
    UI::Container::handleEvents(pInput);
}

BuildableObjectView::~BuildableObjectView()
{
    // TODO Auto-generated destructor stub
}

} /* namespace UI */
