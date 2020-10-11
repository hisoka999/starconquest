/*
 * BuildableObjectView.cpp
 *
 *  Created on: 13.03.2018
 *      Author: stefan
 */

#include "BuildableObjectView.h"
#include "../translate.h"
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
    timeLabel = std::make_shared<UI::Label>(this);
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
    addObject(timeLabel);
    buildingImage->setPos(0, 0);
    buildingImage->setWidth(50);
    buildingImage->setHeight(50);
    SDL_Color black = { 255, 255, 255, 255 };
    SDL_Color red = { 255, 0, 0, 255 };

    titleLabel->setColor(black);
    titleLabel->setPos(55, 5);
    priceLabel->setColor(black);
    priceLabel->setPos(55, 20);
    timeLabel->setColor(black);
    timeLabel->setPos(55, 35);

    //update data
    titleLabel->setText(object->getName());
    priceLabel->setTextF(_("Ressources: %d"), object->getRessources());
    timeLabel->setTextF(_("Time: %d months"), object->getRessources());
    buildingImage->setImage(object->getTexture());
}
void BuildableObjectView::update(const int planetRessources)
{
    timeLabel->setTextF(_("Time: %d months"), object->getRessources() / planetRessources);
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
