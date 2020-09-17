#include "engine/ui/Button.h"
#include <engine/graphics/TextureManager.h>
#include <functional>

namespace UI {

Button::Button(Object* parent)
    : Object(parent)
    , static_width(0)
    , hover(false)
    , enabled(true)
    , borderless(false)
{
    color = { 255, 255, 255, 0 };
    width = 0;

    texture = graphics::TextureManager::Instance().loadTexture("images/Button01.png");
}

Button::~Button()
{
}

void Button::setStaticWidth(const int pWidth)
{
    static_width = pWidth;
    if (static_width > width) {
        width = static_width;
        setWidth(width);
    }
}

void Button::setFont(const std::string& fontname, unsigned int font_size)
{
    //TODO error handling
    graphics::Text* text = new graphics::Text();
    text->openFont(fontname, font_size);
    Object::setFont(text);
}
void Button::setColor(SDL_Color color)
{
    this->color = color;
}

void Button::setLabel(const std::string& label)
{
    this->label = label;

    int w = 0;
    int h = 0;
    if (getFont()->size(label, &w, &h) != -1) {
        if (w > static_width) {
            width = w;
        } else {
            width = static_width;
        }
        setWidth(width);
        setHeight(35);
    }
}
graphics::Rect Button::displayRect()
{
    graphics::Rect r;

    if (getParent() != nullptr) {
        r = getParent()->displayRect();

        r.x += getX();
        r.y += getY();
    } else {
        r.x = getX();
        r.y = getY();
    }
    r.width = width + 25;
    r.height = getHeight();
    return r;
}

bool Button::getBorderless() const
{
    return borderless;
}

void Button::setBorderless(bool value)
{
    borderless = value;
}
void Button::handleEvents(core::Input* pInput)
{
    if (!enabled) {
        hover = false;
        return;
    }
    graphics::Rect r = displayRect();

    if (r.intersects(pInput->getMousePostion())) {
        if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT)) {
            this->fireFuncionCall(buttonClickCallback());
        }
        hover = true;
    } else {
        hover = false;
    }
    Object::handleEvents(pInput);
}

void Button::render(core::Renderer* pRender)
{

    int tx;
    SDL_Color displayColor = color;

    if (!enabled) {
        displayColor = { 204, 204, 204, 255 };
    } else if (hover) {
        displayColor = { 93, 103, 108, 255 };
    }

    int ty;
    if (getParent() != nullptr) {
        graphics::Rect r = getParent()->displayRect();

        r.x += getX();
        r.y += getY();
        tx = r.x;
        ty = r.y;
    } else {
        tx = getX();
        ty = getY();
    }

    if (!borderless) {
        //draw background rect
        graphics::Rect backgroundRect;
        backgroundRect.x = tx;
        backgroundRect.y = ty;
        backgroundRect.width = getWidth() + 25;
        backgroundRect.height = getHeight();

        pRender->setDrawColor(12, 21, 24, 255);
        pRender->fillRect(backgroundRect);

        //left top corner

        texture->render(pRender, tx, ty, 9, 9, 0, 0);
        //left bottom corner
        texture->render(pRender, tx, ty + getHeight() - 9, 9, 9, 0, 20);

        texture->render(pRender, tx + getWidth() + 25 - 9, ty, 9, 9, 174, 0);
        texture->render(pRender, tx + getWidth() + 25 - 9, ty + getHeight() - 9, 9, 9, 174, 20);
    }
    getFont()->render(pRender, label, displayColor, tx + 10, ty + 10);
}

void Button::enable()
{
    enabled = true;
}
void Button::disable()
{
    enabled = false;
}
bool Button::isEnabled()
{
    return enabled;
}

} // namespace UI