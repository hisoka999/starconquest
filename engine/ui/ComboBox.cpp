#include "engine/ui/ComboBox.h"

namespace UI {

ComboBox::ComboBox(Object* parent)
    : UI::Object(parent)
    , selection(0)
{
    width = 100;
    //texture = new CTexture("ArkanaLook.png");
    //border = texture->subSurface(7,217,6,6);
    //background = texture->subSurface(250,80,50,50);
    //border->zoomImage(width/6.0,28.0/6.0);
    mouseDown = false;
    renderOrder = 99;
}

ComboBox::~ComboBox()
{
}

void ComboBox::handleEvents(core::Input* pInput)
{
    SDL_Event e = pInput->getEvent();
    int tx = getX();

    int ty = getY();

    if (this->getParent()) {
        graphics::Rect dsp = getParent()->displayRect();
        tx += dsp.x;
        ty += dsp.y;
    }
    int height = 28;
    if (mouseDown && elements.size() > 0) {
        height *= elements.size();
    }
    if (e.button.x >= tx && e.button.x <= tx + width + 28 && e.button.y >= ty
        && e.button.y <= ty + height) {
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {

            mouseDown = true;
        } else if (e.type == SDL_MOUSEBUTTONUP
            && e.button.button == SDL_BUTTON_LEFT) {
            mouseDown = false;
            setSelection((e.button.y - ty) / 28);
        }
    } else {
        mouseDown = false;
    }
}

void ComboBox::render(core::Renderer* pRender)
{

    int tx = getX();

    int ty = getY();

    if (this->getParent()) {
        graphics::Rect dsp = getParent()->displayRect();
        tx += dsp.x;
        ty += dsp.y;
    }
}

void ComboBox::setSelectionByText(std::string text)
{
    // Get index of element from iterator
    int index = -1;
    auto it = std::find(elements.begin(), elements.end(), text);
    if (it != elements.end()) {
        index = std::distance(elements.begin(), it);
        setSelection(index);
    }
}

} // namespace UI
