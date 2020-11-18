#include "shiplistview.h"
#include <engine/graphics/TextureManager.h>

namespace UI {
ShipListView::ShipListView(const std::shared_ptr<Fleet>& fleet, UI::Object* parent)
    : UI::Object(parent)
    , fleet(fleet)
    , selectedShip(nullptr)
{
    background = graphics::TextureManager::Instance().loadTexture("images/Cells01.png");

    setWidth(background->getWidth());
    setHeight(background->getHeight());
}

ShipListView::~ShipListView()
{
}
void ShipListView::render(core::Renderer* pRender)
{
    auto rect = displayRect();
    // render background
    background->render(pRender, rect.x, rect.y);
    //render ships
    int i = 0;
    int x = rect.x;
    int y = rect.y;
    for (auto& ship : fleet->getShips()) {
        auto tex = ship->getTexture();

        tex->renderResized(pRender, x + 3 + (i * 53), y + 3, 50, 50);

        if (i == 2) {
            i = 0;
            y += 53;
        }
        i++;
    }
}
void ShipListView::handleEvents(core::Input* pInput)
{
    auto rect = displayRect();
    int i = 0;
    float x = rect.x;
    float y = rect.y;
    for (auto& ship : fleet->getShips()) {
        auto tex = ship->getTexture();

        graphics::Rect shipRect = { x + (i * 53), y, 53, 53 };
        if (shipRect.intersects(pInput->getMousePostion())
            && pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            selectedShip = ship;
            fireFuncionCall("shipSelected", selectedShip);
        }

        if (i == 2) {
            i = 0;
            y += 53;
        }
        i++;
    }
}

std::shared_ptr<Ship> ShipListView::getSelectedShip() const
{
    return selectedShip;
}
}
