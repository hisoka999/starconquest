#include "buildqueueobjectview.h"

#include "../translate.h"
#include <engine/utils/os.h>

namespace UI {

BuildQueueObjectView::BuildQueueObjectView(BuildQueueElement* object,
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

void BuildQueueObjectView::buildUI()
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
    titleLabel->setText(object->object->getLocalisedName());
    priceLabel->setTextF(_("Ressources: %d"), object->resourcesLeft);
    timeLabel->setTextF(_("Time: %d months"), object->resourcesLeft);
    buildingImage->setImage(object->object->getTexture());
}
void BuildQueueObjectView::update(const int planetRessources)
{
    float value = std::round(static_cast<float>(object->resourcesLeft) / static_cast<float>(planetRessources));
    timeLabel->setTextF(_("Time: %.0f months"), value);
}

void BuildQueueObjectView::render(core::Renderer* pRender)
{
    UI::Container::render(pRender);
}
void BuildQueueObjectView::handleEvents(core::Input* pInput)
{
    UI::Container::handleEvents(pInput);
}

BuildQueueObjectView::~BuildQueueObjectView()
{
    // TODO Auto-generated destructor stub
}

} /* namespace UI */
